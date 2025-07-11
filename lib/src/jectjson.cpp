/*-- File description -------------------------------------------------------*/
/**
 *   @file:    JectJson.cpp
 *
 *   @author:  nikita.lutsenko
 *
 */

#include <jectjson.h>

 /*-- Other libraries --------------------------------------------------------*/
 /*-- Hardware specific libraries --------------------------------------------*/
 /*-- Project specific includes ----------------------------------------------*/

#include <rapidjson/prettywriter.h>
#include <rapidjson/filewritestream.h>

/*-- Standard C/C++ Libraries -----------------------------------------------*/

#include <sstream>
#include <sys/stat.h>

/*-- Imported functions -----------------------------------------------------*/
/*-- Local namespace --------------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local Variables --------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Class function ---------------------------------------------------------*/

JsonCpp::JsonCpp() {
    this->json_path = nullptr;
    this->pEtal = nullptr;
    this->json_access = FileAccess::R;
    this->userCallbacks.onError = nullptr;
    this->has_open_json = false;
}

JsonCpp::~JsonCpp() {
    this->pEtal = nullptr;
}

void JsonCpp::Update() {
    this->pEtal = &this->curr_obj;
}

bool JsonCpp::Save() {

    {
        bool result = false;

        if((this->json_path == nullptr) || (strlen(this->json_path) == 0)) {
			this->errorHandler("function save", "not set save path");
            return result;
		}

       
        try {

            assert(&this->file_mutex != nullptr);
			this->file_mutex.lock();

            if(this->json_access != FileAccess::R) {

                if(std::ifstream(this->json_path)) {
                    this->file_stream.open(this->json_path, std::ios::trunc | std::ios::out);
                    this->file_stream << "{" << std::endl << std::endl << "}";
                    this->file_stream.close();
                }
                rapidjson::StringBuffer buffer;
                rapidjson::PrettyWriter<rapidjson::StringBuffer> wr(buffer);
                this->curr_obj.Accept(wr);
                const char *data_out = buffer.GetString();
                this->file_stream.open(this->json_path, std::ios::out);
                this->file_stream << data_out;
                this->file_stream.close();
                result = true;
            }

            this->file_mutex.unlock();

        } catch(...) {

			this->file_mutex.unlock();
            this->errorHandler("ERROR", "Save file");
            result = false;
        }

        this->has_open_json = false;
		return result;
    }
}

void JsonCpp::PrintIt() {

    if (!this->has_open_json) {
        this->errorHandler("function Key: JSON is not open!:", "");
        return;
    }

    char writeBuffer[65536];
    rapidjson::FileWriteStream os(stdout, writeBuffer, sizeof(writeBuffer));
    rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
    this->curr_obj.Accept(writer);
    printf("\n");
}

uint32_t JsonCpp::GetChildObjectCount() {
    this->pEtal = &this->curr_obj;
    return (*this->pEtal).MemberCount();
}

std::string JsonCpp::GetStringDataBuffer() {

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> wr(buffer);
    this->curr_obj.Accept(wr);

    return std::move(buffer.GetString());
}

bool JsonCpp::SetSringDataBuffer(const char* DataStrBuff) {

    if(DataStrBuff == nullptr) {
        return false;
	}

    this->document.Parse(DataStrBuff);
    this->current_object = this->document.GetObject();
    rapidjson::Document::AllocatorType &a = document.GetAllocator();
    this->etalon_obj.CopyFrom(this->current_object, a);
    this->pEtal = &this->curr_obj;
    this->has_open_json = true;
    return true;
}

std::string JsonCpp::GetArray(const char* key_val) {
    return std::string();
}

std::string JsonCpp::GetElementType(const rapidjson::Value &val) {

    if(val.GetType() == rapidjson::Type::kNumberType) {
        if(val.IsFloat()) {
            return std::to_string(val.GetFloat());
        }
        if(val.IsInt()) {
            return std::to_string(val.GetInt());
        }
        if(val.IsBool()) {
            return std::to_string(val.GetBool());
        }
        return "NUM?";

    }
    if(val.GetType() == rapidjson::Type::kStringType) {
        return val.GetString();
    }
    if(val.GetType() == rapidjson::Type::kArrayType) {
        return "Array";
    }
    if(val.GetType() == rapidjson::Type::kObjectType) {
        return "Object";
    }
    return "Unknown";
}

JsonCpp& JsonCpp::Key(const char *key_name) {

    if (!this->has_open_json) {
        this->errorHandler("function Key: JSON is not open!:", key_name);
        return *this;
    }

    try {

        std::string temp_key_name = key_name;
        rapidjson::Value &te = *this->pEtal;
        rapidjson::Value::ConstMemberIterator Iter = te.MemberBegin();

        for(; Iter != te.MemberEnd(); ++Iter) {
            if(temp_key_name.compare(Iter->name.GetString()) == 0) {
                if(Iter->value.IsObject()) {
                    this->pEtal = &te[Iter->name.GetString()];
                    return *this;
                }
                if(Iter->value.IsArray()) {
                    if(Iter->value[0].IsObject()) {
                        this->pEtal = &te[Iter->name.GetString()][0];
                        return *this;
                    }
                    throw "KEY-ARR - IS NOT OBJECT";
                }
                throw "KEY-ARR - IS NOT ARRAY()";
            }          
        }

        throw "KEY-ARR - NOT FOUND KEY: ";

    } catch(const char *message) {
        std::string temp = message;
        temp += key_name;
		this->errorHandler("function key", temp.c_str());
        return *this;
    }
}

JsonCpp &JsonCpp::Key(const char *key_name, uint32_t obj_array_index) {

    if (!this->has_open_json) {
		this->errorHandler("function Key: JSON is not open!:", key_name);
		return *this;
	}

  try {
        std::string temp_key_name = key_name;
        rapidjson::Value &te = *this->pEtal;
        rapidjson::Value::ConstMemberIterator Iter = te.MemberBegin();

        for(; Iter != te.MemberEnd(); ++Iter) {
            if(temp_key_name.compare(Iter->name.GetString()) == 0) {
                if(Iter->value.IsObject()) {
                    this->pEtal = &te[Iter->name.GetString()];
                    return *this;
                } 
				else if(Iter->value.IsArray()) {
                    if(Iter->value[obj_array_index].IsObject()) {
                        this->pEtal = &te[Iter->name.GetString()][obj_array_index];
                        return *this;
                    }
                    throw "KEY-ARR - IS NOT OBJECT";
                }
                throw "KEY-ARR - IS NOT ARRAY()";
            }
        }

        throw "KEY-ARR - NOT FOUND KEY: ";

    } catch(const char *message) {
        std::string temp = message;
        temp += key_name;
        this->errorHandler("function key", temp.c_str());
    }

    return *this;
}

void JsonCpp::RemoveKey(const char *key_val) {

    if (!this->has_open_json) {
        this->errorHandler("function RemoveKey: JSON is not open!:", key_val);
        return;
    }

    {
        std::string key_name = key_val;
        rapidjson::Value &temp = *this->pEtal;
        rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();

        bool exist_key = false;

        for(; current_iterator != temp.MemberEnd(); ++current_iterator) {
            if(key_name.compare(current_iterator->name.GetString()) == 0) {
                exist_key = true;
                temp.EraseMember(current_iterator->name.GetString());
                this->pEtal = &this->curr_obj;
                break;
            }
        }

        if (!exist_key) {
            this->errorHandler("function GetValue: NOT FOUND KEY:", key_name.c_str());
        }

    }
}

void JsonCpp::RemoveKey(const char *key_val, uint32_t size_data) {

}

bool JsonCpp::Save(const char* pathFile) {

    bool result = false;

    {
        if(this->json_path == nullptr) {
            return result;
		}

        try {

			this->file_mutex.lock();

            if(this->json_access != FileAccess::R) {
                if(std::ifstream(pathFile)) {
                    this->file_stream.open(this->json_path, std::ios::trunc | std::ios::out);
                    this->file_stream << "{" << std::endl << std::endl << "}";
                    this->file_stream.close();
                }
                rapidjson::StringBuffer buffer;
                rapidjson::PrettyWriter<rapidjson::StringBuffer> wr(buffer);
                this->curr_obj.Accept(wr);
                const char *data_out = buffer.GetString();
                this->file_stream.open(pathFile, std::ios::out);
                this->file_stream << data_out;
                this->file_stream.close();
                result = true;

            }

            this->file_mutex.unlock();

        } catch(...) {

            this->file_mutex.unlock();
            this->errorHandler("ERROR", "save file");
            result = false;
        }
    }

    this->has_open_json = false;
    return result;
}

bool JsonCpp::Open(const char *json_path, FileAccess jsonAccess) {

    {

        assert(&file_mutex != nullptr);
     
        this->file_mutex.lock();

        bool fileExist = false;

        if (std::ifstream(json_path)) {
            fileExist = true;

            if (!this->IsValidJson(json_path)) {
                return false;
                this->file_mutex.unlock();
            }
        }

        bool result = false;
        rapidjson::IStreamWrapper* istream_wraper = nullptr;
        this->json_access = jsonAccess;
        this->json_path = json_path;


        try {

            switch(this->json_access) {
                case FileAccess::R:
                case FileAccess::RW:
                {
                    if(fileExist) {
                        this->file_stream.open(this->json_path, std::ios::in);
                        this->has_open_json = true;
                    } else {
                        throw "Json File is not found! Path: ";
                    }
                }
                break;
                case FileAccess::RWA:
                {
                    if(!fileExist) {
                        this->file_stream.open(this->json_path, std::ios::out);
                        this->file_stream << "{" << std::endl << std::endl << "}";
                        this->file_stream.close();
                    }

                    this->file_stream.open(this->json_path, std::ios::in);
                    this->has_open_json = true;
                }
                break;
                case FileAccess::RWAS:
                {
                    if(!fileExist) {
                        this->file_stream.open(this->json_path, std::ios::out);
                        this->file_stream << "{" << std::endl << std::endl << "}";
                        this->file_stream.close();

                    } else {
                        remove(this->json_path);
                        this->file_stream.open(this->json_path, std::ios::out);
                        this->file_stream << "{" << std::endl << std::endl << "}";
                        this->file_stream.close();
                    }

                    this->file_stream.open(this->json_path);
                    this->has_open_json = true;
                }
                break;
                case FileAccess::VIRTUAL:
                {
                    this->document.Parse("{}");
                    this->has_open_json = true;
                }
                break;
                default:
                    throw "FILE ACCESS IS UNKNOWN!";
                break;
            }

            if(this->json_access != FileAccess::VIRTUAL) {
                istream_wraper = new rapidjson::IStreamWrapper(this->file_stream);

                if(istream_wraper) {
                    this->document.ParseStream(*(istream_wraper));
                    delete istream_wraper;
                }

                this->file_stream.close();
            }

            this->current_object = this->document.GetObject();
            rapidjson::Document::AllocatorType &a = document.GetAllocator();
            this->etalon_obj.CopyFrom(this->current_object, a);
            this->pEtal = &this->curr_obj;           
            this->file_mutex.unlock();
            result = true;

        } catch(const char *message) {

            std::string temp = message;
            temp += json_path;

            if(istream_wraper != nullptr) {
                delete istream_wraper;
            }
            this->file_mutex.unlock();
            this->errorHandler("Func Open", temp.c_str());
            result = false;
            this->has_open_json = false;
        }

        return result;
    }
}

bool JsonCpp::Open(const std::string& json_path, FileAccess jsonAccess) {

    this->json_file_path = json_path;
    this->json_path = this->json_file_path.c_str();
    return this->Open(this->json_path, jsonAccess);
}

uint32_t JsonCpp::GetSizeArray(const char *key_val) {

    if (!this->has_open_json) {
        this->errorHandler("function GetValue: JSON is not open!:", key_val);
        return 0;
    }

    {
        uint32_t res = 0;
        std::string key_name = key_val;
        rapidjson::Value &temp = *this->pEtal;
        rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();

        for(; current_iterator != temp.MemberEnd(); ++current_iterator) {
            if(key_name.compare(current_iterator->name.GetString()) == 0) {
                if(current_iterator->value.IsArray()) {
                    res = temp[current_iterator->name.GetString()].Size();
                } else {
                    res = 1;
                }
                break;
            }
        }

        this->pEtal = &this->curr_obj;
        return res;
    }
}

bool JsonCpp::RegisterCallback_onError(const ErrorCallback& callbackFunc) {
    if(callbackFunc) {
        this->userCallbacks.onError = callbackFunc;
		return true;
    }
    return false;
}

bool JsonCpp::RegisterCallback_onError(const ErrorCallback2& callbackFunc) {
    if(callbackFunc) {
        this->userCallbacks.onError2 = callbackFunc;
        return true;
    }
    return false;
}

void JsonCpp::errorHandler(const char *message, const char *details) {
    if(this->userCallbacks.onError) {
        this->userCallbacks.onError(message, details);
	} if(this->userCallbacks.onError2) {
        this->userCallbacks.onError2("JSONCPP ",message,details);
    }
}

void JsonCpp::errorHandler(const std::string& modul_name,const std::string& message,const std::string& details) {
	if(this->userCallbacks.onError2) {
        this->userCallbacks.onError2(modul_name,message,details);
	} else if(this->userCallbacks.onError) {
        this->userCallbacks.onError(message.c_str(), details.c_str());
	}
}

bool JsonCpp::IsValidJson(const std::string& json_file) {

    std::ifstream file(json_file);
    if (!file) {
        this->errorHandler("IsValidJson", " ", "Cannot open file: " + json_file);
    }
    
    std::ostringstream ss;
    ss << file.rdbuf();
    std::string js_file = ss.str();

    rapidjson::Document doc;
    doc.Parse(js_file.c_str());

    if (doc.HasParseError()) {
        int error_temp = doc.GetParseError();
        this->errorHandler("JSON parse error, invalid json file", std::to_string(error_temp).c_str());
        return false;
    }
    return true;
}

/*-- Exported functions -----------------------------------------------------*/
/*-- EOF --------------------------------------------------------------------*/