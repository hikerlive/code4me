#include "SSCommon.h"

#include "ssdatadomain.pb.h"
class auto_protobuf_lib {
public: auto_protobuf_lib(){}
		~auto_protobuf_lib(){::google::protobuf::ShutdownProtobufLibrary();}
} __auto_protobuf_lib__;

//class auto_log_module {
//public: auto_log_module(){}
//		~auto_log_module(){SSLogModule::dispose();}
//} __auto_log_module__;