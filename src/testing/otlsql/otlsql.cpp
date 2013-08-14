#include <common/ParseOptions.hpp>
#include <system/System.hpp>

#include <network/Service.hpp>

int submain(int argc, char ** argv)
{
	TRACE_FUNCTION
	try
	{
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);

		LOG << Network::HostInfo::getLocalIp();
	}
	catch(otl_exception& p)
	{
		LOG << "OTL Exception (SQL) : " + Common::toString(p.msg) + ", " + p.stm_text + ", " + p.var_info;
	}
	catch(Common::Exception&)
	{
	}
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN
