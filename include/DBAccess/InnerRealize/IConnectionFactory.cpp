#include "IConnectionFactory.h"

using	namespace DBAccessModule;

// ����������ID
INT		IConnectionFactory::m_iAutoConnectID = coniDefaultConnectionAutoId;

IConnectionFactory::IConnectionFactory(void)
{
	m_iAutoConnectID = coniDefaultConnectionAutoId;
}

IConnectionFactory::~IConnectionFactory(void)
{
}


