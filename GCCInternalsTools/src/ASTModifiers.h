

namespace GCCInternalsTools
{
	Result		AddNamespace( CPPModel::ASTDictionary&						dictionary,
							  const std::string&							namespaceToAdd );

	Result		AddGlobalVar( CPPModel::ASTDictionary&						dictionary,
							  const CPPModel::GlobalVarDeclaration&			globalDecl );
}
