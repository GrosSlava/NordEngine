
#include "GenerationInfo.h"
#include "HelpFunctions.h"





inline std::string CXString2String(const CXString& Str)
{
	std::string LResult = clang_getCString(Str);
	clang_disposeString(Str);
	return LResult;
}

inline FReflectionGenInfo* AsGenInfo(CXClientData ClientData)
{
	return static_cast<FReflectionGenInfo*>(ClientData);
}

inline std::string GetCursorValueName(const CXCursor& C)
{
	return CXString2String(clang_getCursorSpelling(C));
}

inline std::string GetCursorTypeName(const CXCursor& C)
{
	return CXString2String(clang_getTypeSpelling(clang_getCursorType(C)));
}

inline std::string GetCursorEnumTypeName(const CXCursor& C)
{
	return CXString2String(clang_getTypeSpelling(clang_getEnumDeclIntegerType(C)));
}

inline std::string GetCursorResultTypeName(const CXCursor& C)
{
	return CXString2String(clang_getTypeSpelling(clang_getCursorResultType(C)));
}

inline bool IsAbstract(const CXCursor& C)
{
	return clang_CXXRecord_isAbstract(clang_getTypeDeclaration(clang_getCursorType(C)));
}





CXChildVisitResult DebugVisitor(CXCursor C, CXCursor Parent, CXClientData ClientData)
{
	if( clang_Location_isFromMainFile(clang_getCursorLocation(C)) == 0 ) return CXChildVisit_Continue;

	const unsigned int CurLevel = *(reinterpret_cast<unsigned int*>(ClientData));
	unsigned int NextLevel = CurLevel + 1;

	std::cout << std::string(CurLevel, '-');
	std::cout << CXString2String(clang_getCursorKindSpelling(clang_getCursorKind(C)));
	std::cout << "(" << CXString2String(clang_getCursorSpelling(C)) << ")";
	std::cout << "[ ";
	std::cout << "Parent=" << CXString2String(clang_getCursorSpelling(clang_getCursorSemanticParent(C)));
	std::cout << " Type=" << CXString2String(clang_getTypeSpelling(clang_getCursorType(C)));
	std::cout << " ]\n";

	clang_visitChildren(C, DebugVisitor, &NextLevel);

	return CXChildVisit_Continue;
}

void FillGenInfo(FReflectionGenInfo& GenInfo, const CXTranslationUnit& TranslationUnit)
{
	CXCursor RootCursor = clang_getTranslationUnitCursor(TranslationUnit);

#if DEBUG_PRINT_TREE
	unsigned int LTreeLevel = 0;
	clang_visitChildren(RootCursor, DebugVisitor, &LTreeLevel);
#endif

	clang_visitChildren(
		RootCursor,
		[](CXCursor C, CXCursor Parent, CXClientData ClientData)
		{
			if( clang_Location_isFromMainFile(clang_getCursorLocation(C)) == 0 ) return CXChildVisit_Continue;

			switch( clang_getCursorKind(C) )
			{
			case CXCursor_ClassDecl:
			{
				struct ClassContext
				{
					FClassInfo CurrentClassInfo;
					CXCursor CurrentClass = CXCursor();
				} LClassContext;
				LClassContext.CurrentClassInfo.ClassName = GetCursorTypeName(C);
				LClassContext.CurrentClass = C;

				clang_visitChildren(
					C,
					[](CXCursor C, CXCursor Parent, CXClientData ClientData)
					{
						ClassContext* LClassContext = static_cast<ClassContext*>(ClientData);

						if( clang_getCursorKind(C) == CXCursor_CXXBaseSpecifier )
						{
							clang_visitChildren(
								C,
								[](CXCursor C, CXCursor Parent, CXClientData ClientData)
								{
									if( clang_getCursorKind(C) != CXCursor_TypeRef ) return CXChildVisit_Break;

									ClassContext* LClassContext = static_cast<ClassContext*>(ClientData);
									if( IsAbstract(C) )
									{
										LClassContext->CurrentClassInfo.Interfaces.push_back(GetCursorTypeName(C));
									}
									else
									{
										LClassContext->CurrentClassInfo.ParentName.push_back(GetCursorTypeName(C));
									}
									return CXChildVisit_Break;
								},
								ClientData);
						}
						else if( clang_getCursorKind(C) == CXCursor_FieldDecl )
						{
							FFieldInfo LFieldInfo;
							LFieldInfo.Name = GetCursorValueName(C);
							LFieldInfo.AccessSpecifier = static_cast<EAccessSpecifier>(clang_getCXXAccessSpecifier(C));

							LClassContext->CurrentClassInfo.Fields.push_back(LFieldInfo);
						}
						else if( clang_getCursorKind(C) == CXCursor_CXXMethod )
						{
							FMethodInfo LMethodInfo;
							LMethodInfo.Name = GetCursorValueName(C);
							LMethodInfo.AccessSpecifier = static_cast<EAccessSpecifier>(clang_getCXXAccessSpecifier(C));
							LMethodInfo.ResultTypeName = GetCursorResultTypeName(C);

							clang_visitChildren(
								C,
								[](CXCursor C, CXCursor Parent, CXClientData ClientData)
								{
									if( clang_getCursorKind(C) != CXCursor_ParmDecl ) return CXChildVisit_Continue;

									FMethodInfo* LMethodInfo = static_cast<FMethodInfo*>(ClientData);
									LMethodInfo->ArgsTypesName.push_back(GetCursorTypeName(C));

									return CXChildVisit_Continue;
								},
								&LMethodInfo);

							LClassContext->CurrentClassInfo.Methods.push_back(LMethodInfo);
						}

						return CXChildVisit_Continue;
					},
					&LClassContext);

				LClassContext.CurrentClassInfo.Fixup();
				AsGenInfo(ClientData)->ClassesInfo.insert(std::make_pair(LClassContext.CurrentClassInfo.ClassName, LClassContext.CurrentClassInfo));

				return CXChildVisit_Continue;
			}
			case CXCursor_StructDecl:
			{
				struct StructContext
				{
					FStructInfo CurrentStructInfo;
					CXCursor CurrentStruct = CXCursor();
				} LStructContext;
				LStructContext.CurrentStructInfo.StructName = GetCursorTypeName(C);
				LStructContext.CurrentStruct = C;

				clang_Type_visitFields(
					clang_getCursorType(C),
					[](CXCursor FieldC, CXClientData ClientData)
					{
						StructContext* LStructContext = static_cast<StructContext*>(ClientData);

						FFieldInfo LFieldInfo;
						LFieldInfo.Name = GetCursorValueName(FieldC);
						LFieldInfo.AccessSpecifier = static_cast<EAccessSpecifier>(clang_getCXXAccessSpecifier(FieldC));

						LStructContext->CurrentStructInfo.Fields.push_back(std::move(LFieldInfo));

						return CXVisit_Continue;
					},
					&LStructContext);

				LStructContext.CurrentStructInfo.Fixup();
				AsGenInfo(ClientData)->StructsInfo.insert(std::make_pair(LStructContext.CurrentStructInfo.StructName, LStructContext.CurrentStructInfo));

				return CXChildVisit_Continue;
			}
			case CXCursor_EnumDecl:
			{
				struct EnumContext
				{
					FEnumInfo CurrentEnumInfo;
					CXCursor CurrentEnum = CXCursor();
				} LEnumContext;
				LEnumContext.CurrentEnumInfo.EnumName = GetCursorTypeName(C);
				LEnumContext.CurrentEnumInfo.TypeName = GetCursorEnumTypeName(C);
				LEnumContext.CurrentEnumInfo.IsScoped = clang_EnumDecl_isScoped(C);
				LEnumContext.CurrentEnum = C;

				clang_visitChildren(
					C,
					[](CXCursor C, CXCursor Parent, CXClientData ClientData)
					{
						EnumContext* LEnumContext = static_cast<EnumContext*>(ClientData);

						if( clang_getCursorKind(C) != CXCursor_EnumConstantDecl ) return CXChildVisit_Continue;

						FEnumRowInfo LEnumRow;
						LEnumRow.Name = GetCursorValueName(C);
						LEnumRow.Value = clang_getEnumConstantDeclValue(C);

						LEnumContext->CurrentEnumInfo.Rows.push_back(std::move(LEnumRow));

						return CXChildVisit_Continue;
					},
					&LEnumContext);

				LEnumContext.CurrentEnumInfo.Fixup();
				AsGenInfo(ClientData)->EnumsInfo.insert(std::make_pair(LEnumContext.CurrentEnumInfo.EnumName, LEnumContext.CurrentEnumInfo));

				return CXChildVisit_Continue;
			}
			}

			return CXChildVisit_Recurse;
		},
		&GenInfo);
}





void FClassInfo::Fixup()
{
	// clang-format off
	auto MethodsNewEnd = std::remove_if(Methods.begin(), Methods.end(),
		[this](const FMethodInfo& M) 
		{ 
			return M.AccessSpecifier != EAccessSpecifier::Public || M.Name == StrUpper(ClassName) + "_GENERATED_BODY"; 
		});
	// clang-format on
	Methods.erase(MethodsNewEnd, Methods.end());
}

void FStructInfo::Fixup()
{
}

void FEnumInfo::Fixup()
{
	for( size_t i = 0; i < Rows.size(); ++i )
	{
		if( MinValueIndex == -1 || Rows[i].Value < Rows[MinValueIndex].Value )
		{
			MinValueIndex = i;
		}
		if( MaxValueIndex == -1 || Rows[i].Value > Rows[MaxValueIndex].Value )
		{
			MaxValueIndex = i;
		}
	}
}
