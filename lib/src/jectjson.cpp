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

static const char* ErrorCodeStr[] = {
    "None",
    "InvalidJsonFile",
    "FileNotFound",
    "SavePathIsEmpty",
    "ObjectIsEmpty",
    "KeyNotFound",
    "KeyNotObject",
    "KeyNotArray",
    "KeyTypeMismatch",
    "SaveError",
    "AccessError",
    "RapidJsonError",
    "GeneralError"
};

/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Class function ---------------------------------------------------------*/

JsonCpp::JsonCpp() {
    this->json_path = nullptr;
    this->pEtal = nullptr;
    this->json_access = FileAccess::R;
    this->userCallbacks.onError = nullptr;
    this->object_is_empty_ = true;
    this->error_code_ = ErrorCode::None;
}

JsonCpp::~JsonCpp() {
    this->pEtal = nullptr;
}

void JsonCpp::Update() {
    this->pEtal = &this->curr_obj;
}

bool JsonCpp::Save() {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    if (this->object_is_empty_) {
        std::string msg = std::string(" Json object is empty!");
        this->errorHandler(__FUNCTION__, msg, ErrorCode::ObjectIsEmpty);
        return false;
    }

    if (this->error_code_ != ErrorCode::None) {
        return false;
    }

#else

    if (this->object_is_empty_) {
        this->errorHandler(ErrorCode::ObjectIsEmpty);
        return false;
    }

    if (this->error_code_ != ErrorCode::None) {
        return false;
    }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    {
        bool result = false;

        if((this->json_path == nullptr) || (strlen(this->json_path) == 0)) {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER 
        this->errorHandler(__FUNCTION__, "", ErrorCode::SavePathIsEmpty);
#else
		this->errorHandler(ErrorCode::SavePathIsEmpty);
#endif	
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

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

			this->errorHandler(__FUNCTION__, "", ErrorCode::SaveError);
#else
			this->errorHandler(ErrorCode::SaveError);
#endif
            result = false;
        }

		return result;
    }
}

void JsonCpp::PrintIt() {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    if (this->object_is_empty_) {
        std::string msg = std::string(" Json object is empty!");
        this->errorHandler(__FUNCTION__, msg, ErrorCode::ObjectIsEmpty);
        return;
    }

    if (this->error_code_ != ErrorCode::None) {
        return;
    }

#else

    if (this->object_is_empty_) {
        this->errorHandler(ErrorCode::ObjectIsEmpty);
        return;
    }

    if (this->error_code_ != ErrorCode::None) {
        return;
    }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    char writeBuffer[65536];
    rapidjson::FileWriteStream os(stdout, writeBuffer, sizeof(writeBuffer));
    rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
    this->curr_obj.Accept(writer);
    printf("\n");
}

uint32_t JsonCpp::GetChildObjectCount() {


#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    if (this->object_is_empty_) {
        std::string msg = std::string(" Json object is empty!");
        this->errorHandler(__FUNCTION__, msg, ErrorCode::ObjectIsEmpty);
        return false;
    }

    if (this->error_code_ != ErrorCode::None) {
        return false;
    }

#else

    if (this->object_is_empty_) {
        this->errorHandler(ErrorCode::ObjectIsEmpty);
        return false;
    }

    if (this->error_code_ != ErrorCode::None) {
        return false;
    }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    this->pEtal = &this->curr_obj;
    return (*this->pEtal).MemberCount();
}

std::string JsonCpp::GetStringDataBuffer() {


#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    if (this->object_is_empty_) {
        std::string msg = std::string(" Json object is empty!");
        this->errorHandler(__FUNCTION__, msg, ErrorCode::ObjectIsEmpty);
        return "";
    }

    if (this->error_code_ != ErrorCode::None) {
        return "";
    }

#else

    if (this->object_is_empty_) {
        this->errorHandler(ErrorCode::ObjectIsEmpty);
        return "";
    }

    if (this->error_code_ != ErrorCode::None) {
        return "";
    }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

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
    this->object_is_empty_ = false;
    return true;
}

std::string JsonCpp::GetArray(const char* key_name) {
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

JsonCpp& JsonCpp::Key(const char* key_name) {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    if (this->object_is_empty_) {
        std::string msg = std::string(" key - ") + std::string(key_name) + std::string(" Json object is empty!");
        this->errorHandler(__FUNCTION__, msg, ErrorCode::ObjectIsEmpty);
        return *this;
    }

    if (this->error_code_ != ErrorCode::None) {
        return *this;
    }

#else

    if (this->object_is_empty_) {
        this->errorHandler(ErrorCode::ObjectIsEmpty);
        return *this;
    }

    if (this->error_code_ != ErrorCode::None) {
        return *this;
    }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    try {

        std::string key_name_str = key_name;
        rapidjson::Value &te = *this->pEtal;
        rapidjson::Value::ConstMemberIterator Iter = te.MemberBegin();

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

        if (Iter == te.MemberEnd()) {
            std::string msg = std::string(" key - ") + std::string(key_name);
            this->errorHandler(__FUNCTION__, msg, ErrorCode::KeyNotFound);
			return *this;
        }

        auto it = te.FindMember(key_name_str.c_str());
        if (it == te.MemberEnd()) {
            std::string msg = std::string(" key - ") + std::string(key_name);
            this->errorHandler(__FUNCTION__, msg, ErrorCode::KeyNotFound);
			return *this;
        }

#else

        if (Iter == te.MemberEnd()) {
            this->errorHandler(ErrorCode::KeyNotFound);
			return *this;
        }

        auto it = te.FindMember(key_name_str.c_str());
        if (it == te.MemberEnd()) {
            this->errorHandler(ErrorCode::KeyNotFound);
			return *this;
        }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

        for(; Iter != te.MemberEnd(); ++Iter) {
            if(key_name_str.compare(Iter->name.GetString()) == 0) {
                if(Iter->value.IsObject()) {
                    this->pEtal = &te[Iter->name.GetString()];
                    return *this;
                }
                if(Iter->value.IsArray()) {
                    if(Iter->value[0].IsObject()) {
                        this->pEtal = &te[Iter->name.GetString()][0];
                        return *this;
                    }
                    throw ErrorCode::KeyNotObject;
                }
                throw ErrorCode::KeyNotArray;
            }          
        }

    } catch(ErrorCode code) {
        std::string temp = std::to_string(static_cast<int>(code));
        temp += " Key: ";
        temp += std::string(key_name);
#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
        {
            this->errorHandler(__FUNCTION__, temp.c_str(), code);
		}
#else

        this->errorHandler(code);

#endif
    
    }

    return *this;
}

JsonCpp &JsonCpp::Key(const char *key_name, uint32_t obj_array_index) {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    if (this->object_is_empty_) {
        std::string msg = std::string(" key - ") + std::string(key_name) + std::string(" Json object is empty!");
        this->errorHandler(__FUNCTION__, msg, ErrorCode::ObjectIsEmpty);
        return *this;
    }

    if (this->error_code_ != ErrorCode::None) {
        return *this;
    }

#else

    if (this->object_is_empty_) {
        this->errorHandler(ErrorCode::ObjectIsEmpty);
        return *this;
    }

    if (this->error_code_ != ErrorCode::None) {
        return *this;
    }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

  try {
        std::string key_name_str = key_name;
        rapidjson::Value &te = *this->pEtal;
        rapidjson::Value::ConstMemberIterator Iter = te.MemberBegin();

        for(; Iter != te.MemberEnd(); ++Iter) {
            if(key_name_str.compare(Iter->name.GetString()) == 0) {
                if(Iter->value.IsObject()) {
                    this->pEtal = &te[Iter->name.GetString()];
                    return *this;
                } 
				else if(Iter->value.IsArray()) {
                    if(Iter->value[obj_array_index].IsObject()) {
                        this->pEtal = &te[Iter->name.GetString()][obj_array_index];
                        return *this;
                    }
                    throw ErrorCode::KeyNotObject;
                }
                throw ErrorCode::KeyNotArray;
            }
        }

    } catch(ErrorCode code) {
        std::string temp = std::to_string(static_cast<int>(code));
        temp += " Key: ";
        temp += key_name;
#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
        {
            this->errorHandler(__FUNCTION__, temp.c_str(), code);
		}
#else
		this->errorHandler(ErrorCode::GeneralError);
#endif
 
    }

    return *this;
}

void JsonCpp::RemoveKey(const char *key_name) {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    if (this->object_is_empty_) {
        std::string msg = std::string(" key - ") + std::string(key_name) + std::string(" Json object is empty!");
        this->errorHandler(__FUNCTION__, msg, ErrorCode::ObjectIsEmpty);
        return;
    }

    if (this->error_code_ != ErrorCode::None) {
        return;
    }

#else

    if (this->object_is_empty_) {
        this->errorHandler(ErrorCode::ObjectIsEmpty);
        return;
    }

    if (this->error_code_ != ErrorCode::None) {
        return;
    }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    {
        std::string key_name_str = key_name;
        rapidjson::Value &temp = *this->pEtal;
        rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

        if (current_iterator == temp.MemberEnd()) {
            std::string msg = std::string(" key - ") + std::string(key_name);
            this->errorHandler(__FUNCTION__, msg, ErrorCode::KeyNotFound);
            return;
        }

        auto it = temp.FindMember(key_name_str.c_str());
        if (it == temp.MemberEnd()) {
            std::string msg = std::string(" key - ") + std::string(key_name);
            this->errorHandler(__FUNCTION__, msg, ErrorCode::KeyNotFound);
            return;
        }

#else

        if (current_iterator == temp.MemberEnd()) {
            this->errorHandler(ErrorCode::KeyNotFound);
            return;
        }

        auto it = temp.FindMember(key_name_str.c_str());
        if (it == temp.MemberEnd()) {
            this->errorHandler(ErrorCode::KeyNotFound);
            return;
        }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

        for(; current_iterator != temp.MemberEnd(); ++current_iterator) {
            if(key_name_str.compare(current_iterator->name.GetString()) == 0) {
                temp.EraseMember(current_iterator->name.GetString());
                this->pEtal = &this->curr_obj;
                break;
            }
        }
    }
}

void JsonCpp::RemoveKey(const char *key_name, uint32_t size_data) {
       
}

bool JsonCpp::Save(const char* pathFile) {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    if (this->object_is_empty_) {
        std::string msg = std::string(" Json object is empty!");
        this->errorHandler(__FUNCTION__, msg, ErrorCode::ObjectIsEmpty);
        return false;
    }

    if (this->error_code_ != ErrorCode::None) {
        return false;
    }

#else

    if (this->object_is_empty_) {
        this->errorHandler(ErrorCode::ObjectIsEmpty);
        return false;
    }

    if (this->error_code_ != ErrorCode::None) {
        return false;
    }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    if (this->json_path == nullptr) {
#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
		this->errorHandler(__FUNCTION__, pathFile, ErrorCode::SaveError);
#else
		this->errorHandler(ErrorCode::SaveError);
#endif
        return false;
    }

    bool result = false;

    {

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
#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER 
			this->errorHandler(__FUNCTION__, pathFile, ErrorCode::SaveError);
#else
			this->errorHandler(ErrorCode::SaveError);
#endif
            result = false;
        }
    }

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
                this->file_mutex.unlock();
                return false;
                
            }
        }

        if (!fileExist) {
            if(jsonAccess == FileAccess::R || jsonAccess == FileAccess::RW) {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER	
                this->errorHandler(__FUNCTION__, json_path, ErrorCode::FileNotFound);
#else
				this->errorHandler(ErrorCode::FileNotFound);
#endif

                this->file_mutex.unlock();
                return false;
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
                        this->object_is_empty_ = false;
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
                    this->object_is_empty_ = false;
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
                    this->object_is_empty_ = false;
                }
                break;
                case FileAccess::VIRTUAL:
                {
                    this->document.Parse("{}");
                    this->object_is_empty_ = false;
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

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
			this->errorHandler(__FUNCTION__, temp.c_str(), ErrorCode::GeneralError);
#else
			this->errorHandler(ErrorCode::GeneralError);
#endif
            result = false;
            this->object_is_empty_ = true;
        }

        return result;
    }
}

bool JsonCpp::Open(const std::string& json_path, FileAccess jsonAccess) {

    this->json_file_path = json_path;
    this->json_path = this->json_file_path.c_str();
    return this->Open(this->json_path, jsonAccess);
}

uint32_t JsonCpp::GetSizeArray(const char *key_name) {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    if (this->object_is_empty_) {
        std::string msg = std::string(" key - ") + std::string(key_name) + std::string(" Json object is empty!");
        this->errorHandler(__FUNCTION__, msg, ErrorCode::ObjectIsEmpty);
        return 0;
    }

    if (this->error_code_ != ErrorCode::None) {
        return 0;
    }

#else

    if (this->object_is_empty_) {
        this->errorHandler(ErrorCode::ObjectIsEmpty);
        return 0;
    }

    if (this->error_code_ != ErrorCode::None) {
        return 0;
    }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    {
        uint32_t res = 0;
        std::string key_name_str = key_name;
        rapidjson::Value &temp = *this->pEtal;
        rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();

        for(; current_iterator != temp.MemberEnd(); ++current_iterator) {
            if(key_name_str.compare(current_iterator->name.GetString()) == 0) {
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

bool JsonCpp::SetErrorCallback(const ErrorCallback& callbackFunc) {
    if(callbackFunc) {
        this->userCallbacks.onError = callbackFunc;
		return true;
    }
    return false;
}

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
void JsonCpp::errorHandler(const std::string& func_name, const std::string& message, const ErrorCode error) {
    
    if(this->error_code_ == ErrorCode::None) {
        this->error_code_ = error;
	}

    if (this->userCallbacks.onError) {
        this->userCallbacks.onError(func_name, message, error);
    }
}
#else
void JsonCpp::errorHandler(const ErrorCode error) {

    if (this->error_code_ == ErrorCode::None) {
        this->error_code_ = error;
    }

    if (this->userCallbacks.onError) {
        this->userCallbacks.onError(error);
    }
}
#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

bool JsonCpp::IsValidJson(const std::string& json_file) {

    std::ifstream file(json_file);

    if (!file) {
#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
        {
            std::string msg = "Cannot open file: " + json_file;
            this->errorHandler(__FUNCTION__, msg, ErrorCode::FileNotFound);
        }

#else
		this->errorHandler(ErrorCode::FileNotFound);
#endif
    }
    
    std::ostringstream ss;
    ss << file.rdbuf();
    std::string js_file = ss.str();
    file.close();

    rapidjson::Document doc;
    doc.Parse(js_file.c_str());

    if (doc.HasParseError()) {
        int error_temp = doc.GetParseError();
        //this->errorHandler("JSON parse error, invalid json file", std::to_string(error_temp).c_str());

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
        std::string msg = "JSON parse error, invalid json file: " + std::string(json_file) + std::to_string(error_temp);
        this->errorHandler(__FUNCTION__, msg, ErrorCode::InvalidJsonFile);
#else
		this->errorHandler(ErrorCode::InvalidJsonFile);
#endif
        return false;
    }
    return true;
}

bool JsonCpp::HasErrorCode() const {
    if (this->error_code_ != ErrorCode::None) {
        return true;
   }
    return false;
}

int JsonCpp::GetErrorCode() {
    return static_cast<int>(this->error_code_);
}

std::string JsonCpp::GetErrorCodeStr(ErrorCode error) {
    size_t idx = static_cast<size_t>(error);
    if (idx < sizeof(ErrorCodeStr) / sizeof(ErrorCodeStr[0])) {
        return std::string(ErrorCodeStr[idx]);
    }
    return std::move(std::string("UnknownErrorCode!"));
}

bool JsonCpp::ObjectIsEmpty() const {
    return this->object_is_empty_;
}

void JsonCpp::ClearErrorState() {
    this->error_code_ = ErrorCode::None;
}

void JsonCpp::ClearObject() {

    this->json_path = nullptr;
    this->json_file_path = "";
    this->pEtal = nullptr;
    this->json_access = FileAccess::R;
    this->object_is_empty_ = true;
    this->error_code_ = ErrorCode::None;
}

/*-- Exported functions -----------------------------------------------------*/
/*-- EOF --------------------------------------------------------------------*/