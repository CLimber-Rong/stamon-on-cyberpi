/*
	Name: Compiler.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 12:31
	Description: 编译器头文件
*/

#pragma once

#include "Parser.cpp"

namespace stamon::c {
class Compiler {
public:
	FileMap filemap;
	SyntaxScope global_scope;
	ArrayList<SourceSyntax> *src;
	STMException *ex;
	ArrayList<StamonString> *ErrorMsg;
	ArrayList<StamonString> *WarningMsg;
	bool ImportFlag = false;

	Compiler() {
	}

	Compiler(Compiler &right) {
		filemap = right.filemap;
		global_scope = right.global_scope;
		src = right.src;
		ex = right.ex;
		ErrorMsg = right.ErrorMsg;
		ImportFlag = right.ImportFlag;
	}

	Compiler(STMException *e, ArrayList<StamonString> *error_msg, ArrayList<StamonString> *warning_msg)
		: filemap(e)
		, global_scope(e) {

		src = new ArrayList<SourceSyntax>();
		ex = e;
		ErrorMsg = error_msg;
		WarningMsg = warning_msg;
		
	}

	void compile(StamonString filename, bool isSupportImport) {
		ImportFlag = isSupportImport;

		LineReader reader = filemap.mark(filename);

		CATCH {
			return;
		}

		int lineNo = 1;
		Lexer lexer(ex);

		while (reader.isMore()) {
			StamonString text = reader.getLine();

			CATCH {
				return;
			}

			int index = lexer.getLineTok(lineNo, text);

			CATCH {
				THROW_S(
					StamonString((char *) "Error: at \"") 
					+ filename
					+ StamonString((char *) "\": ") 
					+ toString(lineNo) + StamonString((char *) ":")
					+ toString(index) + StamonString((char *) " : ") 
					+ ex->getError()
				)
				ErrorMsg->add(ex->getError());

				ex->isError = false;
			}

			CATCH_WARNING {
				for (int i = 0, len = WARNING.size(); i < len; i++) {
					StamonString warning_msg = WARNING[i];
					WarningMsg->add(
						StamonString((char *) "Warning: at \"") 
						+ filename
						+ StamonString((char *) "\": ") 
						+ toString(lineNo)
						+ StamonString((char *) ": ") + warning_msg
					);
				}
				ex->isWarning = false;
			}

			lineNo++;
		}

		Matcher matcher(lexer, ex);
		Parser *parser = new Parser(
				matcher, ex, global_scope, filename, src, filemap, ErrorMsg, ImportFlag);

		ast::AstNode *node = parser->Parse(); // 语法分析

		CATCH {
			THROW_S(
				StamonString((char *) "Syntax Error: at \"") 
				+ filename
				+ StamonString((char *) "\": ") 
				+ toString(parser->ParsingLineNo)
				+ StamonString((char *) ": ") 
				+ ex->getError()
			)
			ErrorMsg->add(ex->getError());

			ex->isError = false;
		}

		SourceSyntax syntax;
		syntax.program = node;
		syntax.filename = filename;

		src->add(syntax);

		reader.close();
	}
};
} // namespace stamon::c