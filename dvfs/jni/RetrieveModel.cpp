#include "RetrieveModel.h"
#include <sys/system_properties.h>

bool isPhoneNexus5(){
	string model = getModel();
	if(model.compare("Nexus 5") == 0){
		return true;
	} else {
		return false;
	}
}

string getModel(){
	char model_string[PROP_VALUE_MAX+1];
	__system_property_get("ro.product.model", model_string);
	return model_string;
}

