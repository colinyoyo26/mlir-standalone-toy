#include "toy/Dialect.h"
#include "toy/Passes.h"

#include "mlir/Dialect/Linalg/IR/LinalgOps.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

using namespace mlir;

#include <iostream>
namespace {
//===----------------------------------------------------------------------===//
// ToyToLinalg RewritePatterns: Matmul operations
//===----------------------------------------------------------------------===//

struct MatmulOpLowering : public OpRewritePattern<toy::MatmulOp> {
  using OpRewritePattern<toy::MatmulOp>::OpRewritePattern;
  LogicalResult matchAndRewrite(toy::MatmulOp op,
                                PatternRewriter &rewriter) const final {
    auto loc = op.getLoc();
    auto lhs = op.lhs();
    auto rhs = op.rhs();

    auto resultTensorType = op.getType().cast<ShapedType>();
    auto resultElementType = resultTensorType.getElementType();
    auto resultShape = resultTensorType.getShape();

    auto resultMemRefType = MemRefType::get(resultShape, resultElementType);

    auto alloc = rewriter.create<AllocOp>(loc, resultMemRefType);

    auto *parentBlock = alloc->getBlock();
    alloc->moveBefore(&parentBlock->front());

    auto dealloc = rewriter.create<DeallocOp>(loc, alloc);
    dealloc->moveBefore(&parentBlock->back());

    auto newOp = rewriter.create<linalg::MatmulOp>(loc, ValueRange{lhs, rhs},
                                                   ValueRange{alloc});
    rewriter.replaceOp(op, newOp.getOperand(2));

    return success();
  }
};
} // end anonymous namespace

namespace {
struct ToyToLinalgLoweringPass
    : public PassWrapper<ToyToLinalgLoweringPass, FunctionPass> {
  void getDependentDialects(DialectRegistry &registry) const override {
    registry.insert<toy::ToyDialect, linalg::LinalgDialect>();
  }
  void runOnFunction() final;
};
} // end anonymous namespace

void ToyToLinalgLoweringPass::runOnFunction() {
  auto function = getFunction();

  if (function.getName() != "main")
    return;

  if (function.getNumArguments() || function.getType().getNumResults()) {
    function.emitError("expected 'main' to have 0 inputs and 0 results");
    return signalPassFailure();
  }

  OwningRewritePatternList patterns;
  patterns.insert<MatmulOpLowering>(&getContext());

  if (failed(applyPatternsAndFoldGreedily(getFunction(), std::move(patterns))))
    signalPassFailure();
}

std::unique_ptr<Pass> mlir::toy::createLowerToLinalgPass() {
  return std::make_unique<ToyToLinalgLoweringPass>();
}

