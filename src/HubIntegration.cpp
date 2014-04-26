/*
 * HubIntegration.cpp
 *
 *  Created on: 24 avr. 2014
 *      Author: pierre
 */


#include "HubIntegration.h"
#include <bb/pim/unified/unified_data_source.h>
#include <QDebug>
#include <QFile>
#include <QDir>


void initHub() {
/*	uds_context_t udsHandle;
	int retVal = -1;
	int serviceId = 0;
	int status = 0;
	bool async = false;

	QString directory = QDir::homePath() + QLatin1String("/app/public/assets/images");
	qDebug() << directory;

	if ((retVal = uds_init(&udsHandle, async)) == UDS_SUCCESS)
	{
	    char* serviceURL = "c_lib_service";
	    char* libPath = "";

	    if( (retVal = uds_register_client(udsHandle, serviceURL, libPath, directory.toStdString().c_str())) != UDS_SUCCESS)
	    {
	    	qWarning() << "Initialization failed";
	    	return;
	    }

	    if (retVal == UDS_SUCCESS)
	    {
	        serviceId = uds_get_service_id(udsHandle);
	        status = uds_get_service_status(udsHandle);
	    }


	    uds_account_data_t *account_data = uds_account_data_create();
	    uds_account_data_set_id(account_data,1000);
	    uds_account_data_set_name(account_data,"HFR10");
	    uds_account_data_set_icon(account_data,"icon_BW.png");
	    uds_account_data_set_target_name(account_data,"com.HFR10.service");

	    if (0 != (retVal = uds_account_added(udsHandle, account_data)))
	    {
	    	if(0 != (retVal = uds_account_updated(udsHandle, account_data))) {
	    		qWarning() << "Fail to create or update!";
	    		return;
	    	}
	    	//qWarning() << uds_account_removed(udsHandle, 1000);
	    }




	}
*/


}
