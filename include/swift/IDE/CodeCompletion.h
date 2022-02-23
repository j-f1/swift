//===--- CodeCompletion.h - Routines for code completion --------*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2017 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//

#ifndef SWIFT_IDE_CODECOMPLETION_H
#define SWIFT_IDE_CODECOMPLETION_H

#include "CodeCompletionResultType.h"
#include "swift/AST/Identifier.h"
#include "swift/Basic/Debug.h"
#include "swift/Basic/LLVM.h"
#include "swift/Basic/OptionSet.h"
#include "swift/Basic/StringExtras.h"
#include "swift/Frontend/Frontend.h"
#include "swift/IDE/CodeCompletionConsumer.h"
#include "swift/IDE/CodeCompletionContext.h"
#include "swift/IDE/CodeCompletionResult.h"
#include "swift/IDE/CodeCompletionResultSink.h"
#include "swift/IDE/CodeCompletionString.h"
#include "swift/IDE/ImportDepth.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/TrailingObjects.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace swift {
class CodeCompletionCallbacksFactory;
class Decl;
class DeclContext;
class FrontendOptions;
class ModuleDecl;
class SourceFile;

namespace ide {

class CodeCompletionCache;
class CodeCompletionContext;
class CodeCompletionResultBuilder;
struct CodeCompletionResultSink;
struct RequestedCachedModule;

/// A routine to remove code completion tokens from code completion
/// tests.
///
/// \code
/// code-completion-token:
///     '#^' identifier '^#'
/// \endcode
///
/// \param Input test source code.
/// \param TokenName names the token whose position should be returned in
/// \p CompletionOffset.
/// \param CompletionOffset set to ~0U on error, or to a 0-based byte offset on
/// success.
///
/// \returns test source code without any code completion tokens.
std::string removeCodeCompletionTokens(StringRef Input,
                                       StringRef TokenName,
                                       unsigned *CompletionOffset);

template <typename T>
ArrayRef<T> copyArray(llvm::BumpPtrAllocator &Allocator,
                            ArrayRef<T> Arr) {
  T *Buffer = Allocator.Allocate<T>(Arr.size());
  std::copy(Arr.begin(), Arr.end(), Buffer);
  return llvm::makeArrayRef(Buffer, Arr.size());
}

struct SwiftCompletionInfo {
  swift::ASTContext *swiftASTContext = nullptr;
  const swift::CompilerInvocation *invocation = nullptr;
  CodeCompletionContext *completionContext = nullptr;
};

void postProcessResults(MutableArrayRef<CodeCompletionResult *> results,
                        CompletionKind Kind, DeclContext *DC,
                        CodeCompletionResultSink *Sink);


/// Create a factory for code completion callbacks.
CodeCompletionCallbacksFactory *
makeCodeCompletionCallbacksFactory(CodeCompletionContext &CompletionContext,
                                   CodeCompletionConsumer &Consumer);

/// Lookup the top-level code completions from \p module and store them in
/// \p targetSink.
///
/// Results are looked up as if in \p currDeclContext, which may be null.
void lookupCodeCompletionResultsFromModule(CodeCompletionResultSink &targetSink,
                                           const ModuleDecl *module,
                                           ArrayRef<std::string> accessPath,
                                           bool needLeadingDot,
                                           const SourceFile *SF);

} // end namespace ide
} // end namespace swift

#endif // SWIFT_IDE_CODECOMPLETION_H
