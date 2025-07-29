#include<DbgContextControl.hpp>

using namespace Mortis::SysIntVecDbg;

DbgContextControl::DbgContextControl(DbgContext& dbg_ctx):
	_dbg_ctx(dbg_ctx)
{

}


DbgContextControl& DbgContextControl::regHookString() {
	
	return *this;
}