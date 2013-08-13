//#include <log/LogObserver.hpp>
#define OTL_ODBC
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
#define OTL_UNICODE
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <otlv4.h>

#include <iostream>
using namespace std;

int main(int argc, char * argv[])
{
	try
	{
		otl_connect db;
		otl_connect::otl_initialize();
		db.rlogon("root/ts1cl2fs@innocence");
		/*
		otl_cursor::direct_exec
		(
			db,
			"drop table test_tab",
			otl_exception::disabled // disable OTL exceptions
		); // drop table

		otl_cursor::direct_exec
		(
			db,
			"create table test_tab(f1 int, f2 varchar(11))"
		);  // create table

		insert(); // insert records into table
		select(); // select records from table
		*/
		db.logoff(); // disconnect from the database
	}
	catch(otl_exception& p)
	{
		cerr<<p.msg<<endl;
		cerr<<p.stm_text<<endl;
		cerr<<p.var_info<<endl;
	}
	return EXIT_SUCCESS;
}
