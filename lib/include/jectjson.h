/*-- File description -------------------------------------------------------*/
/**
 *   @file:    JectJson.h
 *
 *   @author:  nikita.lutsenko
 * 
 */

#ifndef JECTJSON_H
#define JECTJSON_H

 /*-- Standard C/C++ Libraries -----------------------------------------------*/

#include <string>
#include <fstream>
#include <mutex>
#include <typeinfo>
#include <functional>

/*-- Other libraries --------------------------------------------------------*/

#include <rapidjson/reader.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/allocators.h>
#include <rapidjson/writer.h>

/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/

#define ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported classes -------------------------------------------------------*/

/******************************************************************************
 *  @brief  JsonCpp - Json files framework
 *****************************************************************************/
class JsonCpp {
private:

    template <typename T>
    T getT(std::string, std::string t);
    template <typename T>
    T getT(bool b, std::string t);
    template <typename T>
    T getT(double, std::string);
    template <typename T>
    T getT(float, std::string);
    template <typename T>
    T getT(int, std::string);

    template <typename T>
    T getT(int, int t);
    template <typename T>
    T getT(bool, int);
    template <typename T>
    T getT(double, int);
    template <typename T>
    T getT(float, int);
    template <typename T>
    T getT(std::string, int);

    template <typename T>
    T getT(bool, bool t);
    template <typename T>
    T getT(int, bool);
    template <typename T>
    T getT(double, bool);
    template <typename T>
    T getT(float, bool);
    template <typename T>
    T getT(std::string t, bool b);

    template <typename T>
    T getT(int, double);
    template <typename T>
    T getT(bool, double);
    template <typename T>
    T getT(float, double);
    template <typename T>
    T getT(double, double t);
    template <typename T>
    T getT(std::string, double);

    template <typename T>
    T getT(int, float);
    template <typename T>
    T getT(bool, float);
    template <typename T>
    T getT(double, float);
    template <typename T>
    T getT(float, float t);
    template <typename T>
    T getT(std::string, float);

    template <typename T>
    const T *setT(const char*, const char*);
    template <typename T>
    const T *setT(const char*, std::string t);
    template <typename T>
    const T *setT(const char*, bool);
    template <typename T>
    const T *setT(const char*, double);
    template <typename T>
    const T *setT(const char*, float);
    template <typename T>
    const T *setT(const char*, int);
    template<typename T>
    const T *setT(const char*, uint8_t);
    template<typename T>
    const T *setT(const char*, uint32_t);

    template <typename T>
    T setT(std::string, std::string t);
    template <typename T>
    T setT(std::string, bool);
    template <typename T>
    T setT(std::string, double);
    template <typename T>
    T setT(std::string, float);
    template <typename T>
    T setT(std::string, int);
    template<typename T>
    T setT(std::string, uint8_t);
    template<typename T>
    T setT(std::string, uint32_t);
    template <typename T>
    T setT(std::string, const char* t);

    template <typename T>
    T setT(int, int t);
    template <typename T>
    T setT(int, bool);
    template<typename T>
    T setT(int, uint8_t t);
    template<typename T>
    T setT(int, uint32_t t);
    template <typename T>
    T setT(int, double);
    template <typename T>
    T setT(int, float);
    template <typename T>
    T setT(int, std::string);
    template <typename T>
    T setT(int, const char*);

    template <typename T>
    T setT(bool, bool t);
    template <typename T>
    T setT(bool, int);
    template<typename T>
    T setT(bool, uint8_t);
    template<typename T>
    T setT(bool, uint32_t);
    template <typename T>
    T setT(bool, double);
    template <typename T>
    T setT(bool, float);
    template <typename T>
    T setT(bool, std::string);
    template <typename T>
    T setT(bool, const char*);

    template <typename T>
    T setT(double, int);
    template<typename T>
    T setT(double, uint8_t);
    template<typename T>
    T setT(double, uint32_t);
    template <typename T>
    T setT(double, bool);
    template <typename T>
    T setT(double, float);
    template <typename T>
    T setT(double, double t);
    template <typename T>
    T setT(double, std::string);
    template <typename T>
    T setT(double, const char*);

    template <typename T>
    T setT(float, int);
    template<typename T>
    T setT(float, uint8_t);
    template<typename T>
    T setT(float, uint32_t);
    template <typename T>
    T setT(float, bool);
    template <typename T>
    T setT(float, double);
    template <typename T>
    T setT(float, float t);
    template <typename T>
    T setT(float, std::string);
    template <typename T>
    T setT(float, const char*);

public:

    enum class ErrorCode {
         None = 0,
         InvalidJsonFile,
         FileNotFound,
         SavePathIsEmpty,
         ObjectIsEmpty,
         KeyNotFound,
         KeyNotObject,
         KeyNotArray,
         KeyTypeMismatch,
         SaveError,
         AccessError,
         RapidJsonError,
         GeneralError,
    };

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
    typedef std::function<void(const std::string& func_name, const std::string& message, const ErrorCode error)> ErrorCallback;
#else
	typedef std::function<void(const ErrorCode error)> ErrorCallback;
#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

	typedef struct {
        ErrorCallback onError;
    } userCallbacksInfo;

    enum class FileAccess {
        R,
        RW,
        RWA,
        RWAS,
        VIRTUAL
    };

    JsonCpp();
    ~JsonCpp();

    void Update();

    bool Save();
    bool Save(const char* pathFile);
    bool Open(const char* json_path, FileAccess jsonAccess);
    bool Open(const std::string& json_path, FileAccess jsonAccess);

    JsonCpp &Key(const char* key_name);
    JsonCpp &Key(const char* key_name, uint32_t obj_array_index);

    uint32_t GetChildObjectCount();
    uint32_t GetSizeArray(const char* key_name);

    std::string GetElementType(const rapidjson::Value& val);

    bool SetSringDataBuffer(const char* DataStrBuff);
    std::string GetStringDataBuffer();

    void PrintIt();
    void RemoveKey(const char* key_name);
    bool SetErrorCallback(const ErrorCallback& callbackFunc);

    template <typename TypeOutData>
    TypeOutData GetValue(const char* key_name);
    template <typename TypeOutData>
    TypeOutData GetValue(const char* key_name, uint32_t index_value);

    template <typename TypeData>
    void ChangeValue(const char* key_name, TypeData data);
    template <typename TypeData>
    void ChangeArray(const char* key_name, TypeData* type_data, uint32_t size_array);

    template <typename TypeValue0, typename TypeValue1>
    void AddValue(TypeValue0 &key_name, TypeValue1 key_num1);
    template <typename T0, typename T1>
    void AddArray(T0 &key_name, T1 array_value, int sizeArray);
    template<typename T0>
    void AddObjectsArray(T0 &key_name, int sizeArray);
    template <typename T>
    void AddObject(T &key_name);

    void ClearObject();

    bool HasErrorCode() const;
    int GetErrorCode();
    void ClearErrorState();
    std::string GetErrorCodeStr(ErrorCode error);
	bool ObjectIsEmpty() const;

private:
    
    std::string json_file_path;
    const char *json_path;
    FileAccess json_access;
    std::mutex file_mutex;
    std::fstream file_stream;
    rapidjson::Document document;
    rapidjson::Value current_object;
    rapidjson::Value &curr_obj = current_object;
    rapidjson::Value etalon_obj;
    rapidjson::Value *pEtal;
    userCallbacksInfo userCallbacks;

    bool object_is_empty_;
    ErrorCode error_code_;

	std::string GetArray(const char* key_name);
    void RemoveKey(const char* key_name, uint32_t size_data);

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
    void errorHandler(const std::string& func_name, const std::string& message, const ErrorCode error);
#else
	void errorHandler(const ErrorCode error);
#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    bool IsValidJson(const std::string& json_file);
    
    template<typename InputType, typename OutpuType>
    constexpr inline void getTypeError(const std::string& key_name) {
        if (!::std::is_same<InputType, OutpuType>::value) {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
            std::string msg = std::string("key - ") + key_name + " type is not match";
			this->errorHandler("GetValue: ", msg, ErrorCode::KeyTypeMismatch);
#else
			this->errorHandler(ErrorCode::KeyTypeMismatch);

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
        }
    }
    
};

/*-- Exported variables -----------------------------------------------------*/

template <typename T>
T JsonCpp::getT(std::string, std::string t) {
    return t;
}
template <typename T>
T JsonCpp::getT(int, std::string) {
    return 0;
}
template <typename T>
T JsonCpp::getT(bool b, std::string t) {
    return false;
}
template <typename T>
T JsonCpp::getT(double, std::string) {
    return static_cast<double>(0);
}
template <typename T>
T JsonCpp::getT(float, std::string) {
    return 0.0;
}

template <typename T>
T JsonCpp::getT(int, int t) {
    return t;
}
template <typename T>
T JsonCpp::getT(std::string, int) {
    return "";
}
template <typename T>
T JsonCpp::getT(bool, int) {
    return false;
}
template <typename T>
T JsonCpp::getT(double, int) {
    return static_cast<double>(0);
}
template <typename T>
T JsonCpp::getT(float, int) {
    return static_cast<float>(0);
}


template <typename T>
T JsonCpp::getT(bool, bool t) {
    return t;
}
template <typename T>
T JsonCpp::getT(int, bool) {
    return 0;
}
template <typename T>
T JsonCpp::getT(std::string t, bool b) {
    return "";
}
template <typename T>
T JsonCpp::getT(double, bool) {
    return static_cast<double>(0);
}
template <typename T>
T JsonCpp::getT(float, bool) {
    return 0.0;
}


template <typename T>
T JsonCpp::getT(int, double) {
    return 0;
}
template <typename T>
T JsonCpp::getT(bool, double) {
    return false;
}
template <typename T>
T JsonCpp::getT(std::string, double) {
    return "";
}
template <typename T>
T JsonCpp::getT(float, double) {
    return float();
}
template <typename T>
T JsonCpp::getT(double, double t) {
    return t;
}


template <typename T>
T JsonCpp::getT(float, float t) {
    return t;
}
template <typename T>
T JsonCpp::getT(int, float) {
    return static_cast<int>(0);
}
template <typename T>
T JsonCpp::getT(std::string, float) {
    return "";
}
template <typename T>
T JsonCpp::getT(bool, float) {
    return false;
}
template <typename T>
T JsonCpp::getT(double, float) {
    return static_cast<double>(0);
}


template <typename T>
const T *JsonCpp::setT(const char *, int) {
    return nullptr;
}
template <typename T>
const T *JsonCpp::setT(const char *, uint32_t) {
    return nullptr;
}
template <typename T>
const T *JsonCpp::setT(const char *, uint8_t) {
    return nullptr;
}
template <typename T>
const T *JsonCpp::setT(const char *, std::string) {
    return nullptr;
}
template <typename T>
const T *JsonCpp::setT(const char *, const char *t) {
    return t;
}
template <typename T>
const T *JsonCpp::setT(const char *, bool) {
    return nullptr;
}
template <typename T>
const T *JsonCpp::setT(const char *, double) {
    return nullptr;
}
template <typename T>
const T *JsonCpp::setT(const char *, float) {
    return nullptr;
}


template <typename T>
T JsonCpp::setT(std::string, std::string t) {
    return t;
}
template <typename T>
T JsonCpp::setT(std::string, int) {
    return "";
}
template <typename T>
T JsonCpp::setT(std::string, uint32_t) {
    return "";
}
template <typename T>
T JsonCpp::setT(std::string, uint8_t) {
    return "";
}
template <typename T>
T JsonCpp::setT(std::string, const char *t) {
    return t;
}
template <typename T>
T JsonCpp::setT(std::string, bool) {
    return "";
}
template <typename T>
T JsonCpp::setT(std::string, double) {
    return "";
}
template <typename T>
T JsonCpp::setT(std::string, float) {
    return "";
}


template <typename T>
T JsonCpp::setT(int, int t) {
    return t;
}
template <typename T>
T JsonCpp::setT(int, uint32_t t) {
    return static_cast<int>(t);
}
template <typename T>
T JsonCpp::setT(int, uint8_t t) {
    return static_cast<int>(t);
}
template <typename T>
T JsonCpp::setT(int, std::string) {
    return int();
}
template <typename T>
T JsonCpp::setT(int, const char *) {
    return int();
}
template <typename T>
T JsonCpp::setT(int, bool) {
    return int();
}
template <typename T>
T JsonCpp::setT(int, double) {
    return int();
}
template <typename T>
T JsonCpp::setT(int, float) {
    return int();
}


template <typename T>
T JsonCpp::setT(bool, bool t) {
    return t;
}
template <typename T>
T JsonCpp::setT(bool, int) {
    return bool();
}
template <typename T>
T JsonCpp::setT(bool, uint32_t) {
    return bool();
}
template <typename T>
T JsonCpp::setT(bool, uint8_t) {
    return bool();
}
template <typename T>
T JsonCpp::setT(bool, std::string) {
    return bool();
}
template <typename T>
T JsonCpp::setT(bool, const char *) {
    return T();
}
template <typename T>
T JsonCpp::setT(bool, double) {
    return bool();
}
template <typename T>
T JsonCpp::setT(bool, float) {
    return bool();
}


template <typename T>
T JsonCpp::setT(double, int) {
    return double();
}
template <typename T>
T JsonCpp::setT(double, uint32_t) {
    return double();
}
template <typename T>
T JsonCpp::setT(double, uint8_t) {
    return double();
}
template <typename T>
T JsonCpp::setT(double, bool) {
    return double();
}
template <typename T>
T JsonCpp::setT(double, std::string) {
    return double();
}
template <typename T>
T JsonCpp::setT(double, const char *) {
    return T();
}
template <typename T>
T JsonCpp::setT(double, float) {
    return double();
}
template <typename T>
T JsonCpp::setT(double, double t) {
    return t;
}

template <typename T>
T JsonCpp::setT(float, float t) {
    return t;
}
template <typename T>
T JsonCpp::setT(float, int) {
    return float();
}
template <typename T>
T JsonCpp::setT(float, uint8_t) {
    return float();
}
template <typename T>
T JsonCpp::setT(float, uint32_t) {
    return float();
}
template <typename T>
T JsonCpp::setT(float, std::string) {
    return float();
}
template <typename T>
T JsonCpp::setT(float, const char *) {
    return T();
}
template <typename T>
T JsonCpp::setT(float, bool) {
    return float();
}
template <typename T>
T JsonCpp::setT(float, double) {
    return float();
}

/******************************************************************************
 *  @brief get value from json tree
 *
 *  @param key_val
 *  @param Parameter description.
 *          @arg key name  - search key
 *
 *  @return value
 *****************************************************************************/
template <typename TypeOutData>
TypeOutData JsonCpp::GetValue(const char* key_name) {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
        
        if (this->object_is_empty_) {
            std::string msg = std::string(" key - ") + std::string(key_name) + std::string(" Json object is empty!");
            this->errorHandler(__FUNCTION__, msg, ErrorCode::ObjectIsEmpty);
            return TypeOutData();
        }

        if(this->error_code_ != ErrorCode::None) {
            return TypeOutData();
		}

#else
        if (this->object_is_empty_) {
		    this->errorHandler(ErrorCode::ObjectIsEmpty);
            return TypeOutData();
        }

        if (this->error_code_ != ErrorCode::None) {
            return TypeOutData();
        }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    std::string key_name_str = key_name;
    TypeOutData out = TypeOutData();
    TypeOutData out_type = TypeOutData();
    rapidjson::Value &temp = *this->pEtal;
    rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();


#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
        
    if (current_iterator == temp.MemberEnd()) {
        std::string msg = std::string(" key - ") + std::string(key_name);
        this->errorHandler(__FUNCTION__, msg, ErrorCode::KeyNotFound);
        return TypeOutData();
    }

    {
        auto it = temp.FindMember(key_name_str.c_str());
        if (it == temp.MemberEnd()) {
            std::string msg = std::string(" key - ") + std::string(key_name);
            this->errorHandler(__FUNCTION__, msg, ErrorCode::KeyNotFound);
            return TypeOutData();
        }
    }

#else

    if (current_iterator == temp.MemberEnd()) {
	 this->errorHandler(ErrorCode::KeyNotFound);
        return TypeOutData();
    }

    auto it = temp.FindMember(key_name_str.c_str());
    if (it == temp.MemberEnd()) {
        this->errorHandler(ErrorCode::KeyNotFound);
        return TypeOutData();
    }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    for(; current_iterator != temp.MemberEnd(); ++current_iterator) {
        if(key_name_str.compare(current_iterator->name.GetString()) == 0) {
            if(current_iterator->value.IsBool()) {
                getTypeError<TypeOutData, bool>(key_name_str);
                out = getT<TypeOutData>(out_type, current_iterator->value.GetBool());
            } else if(current_iterator->value.IsInt()) {
                getTypeError<TypeOutData, int>(key_name_str);
                out = getT<TypeOutData>(out_type, current_iterator->value.GetInt());
            } else if(current_iterator->value.IsFloat()) {
                getTypeError<TypeOutData, float>(key_name_str);
                out = getT<TypeOutData>(out_type, current_iterator->value.GetFloat());
            } else if(current_iterator->value.IsString()) {
                getTypeError<TypeOutData, std::string>(key_name_str);
                out = getT<TypeOutData>(out_type, static_cast<std::string>(current_iterator->value.GetString()));
            } else if(current_iterator->value.IsDouble()) {
                getTypeError<TypeOutData, double>(key_name_str);
                out = getT<TypeOutData>(out_type, current_iterator->value.GetDouble());
            } else {

                // TODO refactoring.
                if (!current_iterator->value.IsObject()) {
                    this->errorHandler(__func__, " Key not Object! ", ErrorCode::KeyNotObject);
                    return TypeOutData();
                }

                return TypeOutData();
            }
            break;
        }
    }
    this->pEtal = &this->curr_obj;
    return out;
}

/******************************************************************************
 *  @brief get value ( select in array )
 *
 *  @param key_val
 *  @param index_value
 *  @param Parameter description.
 *          @arg key name
 *          @arg index in array
 *
 *  @return value
 *****************************************************************************/
template <typename TypeOutData>
TypeOutData JsonCpp::GetValue(const char* key_name, uint32_t index_value) {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    if (this->object_is_empty_) {
        std::string msg = std::string(" key - ") + std::string(key_name) + std::string(" Json object is empty!");
        this->errorHandler(__FUNCTION__, msg, ErrorCode::ObjectIsEmpty);
        return TypeOutData();
    }

    if (this->error_code_ != ErrorCode::None) {
        return TypeOutData();
    }

#else

    if (this->object_is_empty_) {
        this->errorHandler(ErrorCode::ObjectIsEmpty);
        return TypeOutData();
    }

    if (this->error_code_ != ErrorCode::None) {
        return TypeOutData();
    }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    std::string key_name_str = key_name;
    TypeOutData out = TypeOutData();
    TypeOutData out_type = TypeOutData();
    rapidjson::Value &temp = *this->pEtal;
    rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    if (current_iterator == temp.MemberEnd()) {
        std::string msg = std::string(" key - ") + std::string(key_name);
        this->errorHandler(__FUNCTION__, msg, ErrorCode::KeyNotFound);
        return TypeOutData();
    }

    auto it = temp.FindMember(key_name_str.c_str());
    if (it == temp.MemberEnd()) {
        std::string msg = std::string(" key - ") + std::string(key_name);
        this->errorHandler(__FUNCTION__, msg, ErrorCode::KeyNotFound);
        return TypeOutData();
    }

#else

    if (current_iterator == temp.MemberEnd()) {
        this->errorHandler(ErrorCode::KeyNotFound);
        return TypeOutData();
    }

    auto it = temp.FindMember(key_name_str.c_str());
    if (it == temp.MemberEnd()) {
        this->errorHandler(ErrorCode::KeyNotFound);
        return TypeOutData();
    }

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    for(; current_iterator != temp.MemberEnd(); ++current_iterator) {
        if(key_name_str.compare(current_iterator->name.GetString()) == 0) {
            if(current_iterator->value.IsArray()) {
                if(current_iterator->value[index_value].IsBool()) {
                    getTypeError<TypeOutData, bool>(key_name_str);
                    out = getT<TypeOutData>(out_type, current_iterator->value[index_value].GetBool());
                } else if(current_iterator->value[index_value].IsInt()) {
                    getTypeError<TypeOutData, int>(key_name_str);
                    out = getT<TypeOutData>(out_type, current_iterator->value[index_value].GetInt());
                } else if(current_iterator->value[index_value].IsFloat()) {
                    getTypeError<TypeOutData, float>(key_name_str);
                    out = getT<TypeOutData>(out_type, current_iterator->value[index_value].GetFloat());
                } else if(current_iterator->value[index_value].IsString()) {
                    getTypeError<TypeOutData, std::string>(key_name_str);
                    out = getT<TypeOutData>(out_type, static_cast<std::string>(current_iterator->value[index_value].GetString()));
                } else if(current_iterator->value[index_value].IsDouble()) {
                    getTypeError<TypeOutData, double>(key_name_str);
                    out = getT<TypeOutData>(out_type, current_iterator->value[index_value].GetDouble());
                } else {
                    return TypeOutData();
                }
            } else {
                this->errorHandler(__func__, " Key not Array", ErrorCode::KeyNotArray);
                out = TypeOutData();
            }
            break;
        }
    }

    this->pEtal = &this->curr_obj;
    return out;
}

/******************************************************************************
 *  @brief  change value
 *
 *  @param key_val
 *  @param data
 *  @param Parameter description.
 *          @arg key_name
 *			@arg value input
 *
 *  @return None
 *****************************************************************************/
template <typename TypeData>
void JsonCpp::ChangeValue(const char* key_name, TypeData data) {

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

    std::string key_name_str = key_name;
    rapidjson::Value &temp = *this->pEtal;
    rapidjson::Document::AllocatorType &alloc = this->document.GetAllocator();
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
            
            rapidjson::Value &r = temp[current_iterator->name.GetString()];

            if(typeid(data) == typeid(std::string)) {
                r.SetString(setT<std::string>(std::string(), data).c_str(), static_cast<int>(setT<std::string>(std::string(), data).length()), alloc);
                this->pEtal = &this->curr_obj;
                break;
            }
            if(typeid(data) == typeid(int)) {
                r.SetInt(setT<int>(int(), data));
                this->pEtal = &this->curr_obj;
                break;
            }
            if(typeid(data) == typeid(double)) {
                r.SetDouble(setT<double>(double(), data));
                this->pEtal = &this->curr_obj;
                break;
            }
            if(typeid(data) == typeid(float)) {
                r.SetFloat(setT<float>(float(), data));
                this->pEtal = &this->curr_obj;
                break;
            }
            if(typeid(data) == typeid(bool)) {
                r.SetBool(setT<bool>(bool(), data));
                this->pEtal = &this->curr_obj;
                break;
            }
            if(typeid(data) == typeid(const char *)) {
                r.SetString(rapidjson::StringRef(setT<char>("char", data)));
                this->pEtal = &this->curr_obj;
                break;
            }

            this->pEtal = &this->curr_obj;
            break;
        }
    }
}

/******************************************************************************
 *  @brief  change array
 *
 *  @param key_val
 *  @param type_data
 *  @param size_array
 *  @param Parameter description.
 *          @arg  key_name
 *			@arg  data - array input
 *
 *  @retval None
 *****************************************************************************/
template <typename TypeData>
void JsonCpp::ChangeArray(const char* key_name, TypeData* type_data, uint32_t size_array) {

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

    rapidjson::Value &temp = *this->pEtal;
    this->RemoveKey(key_name);
    this->pEtal = &temp;
    this->AddArray(key_name, type_data, size_array);
    this->pEtal = &this->current_object;
}

/******************************************************************************
 *  @brief  Add new json tag
 *
 *  @param key_num0
 *  @param key_num1
 *  @param Parameter description.
 *          @arg key name
 *          @arg key value
 *
 *  @return None
 *****************************************************************************/
template <typename TypeValue0, typename TypeValue1>
void JsonCpp::AddValue(TypeValue0 &key_name, TypeValue1 key_num1) {

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

    try {

        std::string key_name_str = key_name;
        rapidjson::Value &temp = *this->pEtal;
        rapidjson::Document::AllocatorType &alloc = this->document.GetAllocator();
        rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();
        rapidjson::Value str;
        str.SetString(key_name_str.c_str(), (rapidjson::SizeType)key_name_str.length(), alloc);
        
        rapidjson::Value val;

        if (typeid(key_num1) == typeid(const char*)) {
            val.SetString(rapidjson::StringRef(setT<char>("char", key_num1)));
            temp.AddMember(str, val, alloc);
        }
        else if (typeid(key_num1) == typeid(std::string)) {
            val.SetString(setT<std::string>(std::string(), key_num1).c_str(), static_cast<int>(setT<std::string>(std::string(), key_num1).length()), alloc);
            temp.AddMember(str, val, alloc);
        }
        else if (typeid(key_num1) == typeid(int)) {
            val.SetInt(setT<int>(int(), key_num1));
            temp.AddMember(str, val, alloc);
        }
        else if (typeid(key_num1) == typeid(double)) {
            val.SetDouble(setT<double>(double(), key_num1));
            temp.AddMember(str, val, alloc);
        }
        else if (typeid(key_num1) == typeid(float)) {
            val.SetFloat(setT<float>(float(), key_num1));
            temp.AddMember(str, val, alloc);
        }
        else if (typeid(key_num1) == typeid(bool)) {
            val.SetBool(setT<bool>(bool(), key_num1));
            temp.AddMember(str, val, alloc);
        }

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        temp.Accept(writer);

    } catch(const char *message) {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

        std::string msg = std::string(message) + std::string("key - ") + std::string(key_name);
        this->errorHandler(__FUNCTION__, msg, ErrorCode::GeneralError);

#else

		this->errorHandler(ErrorCode::GeneralError);

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    }

    this->pEtal = &this->curr_obj;
}

/******************************************************************************
 *  @brief  Add new json tag ( array )
 *
 *  @param key_name
 *  @param array_value
 *  @param sizeArray
 *  @param Parameter description.
 *          @arg key name
 *          @arg array pointer
 *          @arg size array
 *
 *  @retval description
 *****************************************************************************/
template <typename T0, typename T1>
void JsonCpp::AddArray(T0 &key_name, T1 array_value, int sizeArray) {
    
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
    
    try {

        std::string key_name_str = key_name;
        rapidjson::Value &temp = *this->pEtal;
        rapidjson::Document::AllocatorType &alloc = this->document.GetAllocator();
        rapidjson::Value arr(rapidjson::kArrayType);
        rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();

        if (typeid(array_value) == typeid(std::string*)) {
            rapidjson::Value strVal;
            std::string st;
            for (int i = 0; i < sizeArray; i++) {
                st = setT<std::string>(std::string(), array_value[i]);
                strVal.SetString(st.c_str(), (rapidjson::SizeType)st.length(), alloc);
                arr.PushBack(strVal, alloc);
            }
        }
        else if (typeid(array_value) == typeid(int*) || typeid(array_value) == typeid(uint8_t*) || typeid(array_value) == typeid(uint32_t*)) {
            for (int i = 0; i < sizeArray; i++) {
                arr.PushBack(setT<int>(int(), array_value[i]), alloc);
            }
        }
        else if (typeid(array_value) == typeid(float*)) {
            for (int i = 0; i < sizeArray; i++) {
                arr.PushBack(setT<float>(float(), array_value[i]), alloc);
            }
        }
        else if (typeid(array_value) == typeid(double*)) {
            for (int i = 0; i < sizeArray; i++) {
                arr.PushBack(setT<double>(double(), array_value[i]), alloc);
            }
        }
        else if (typeid(array_value) == typeid(bool*)) {
            for (int i = 0; i < sizeArray; i++) {
                arr.PushBack(setT<bool>(bool(), array_value[i]), alloc);
            }
        }
        else {
            throw "Input type.";
        }

        rapidjson::Value str;
        str.SetString(key_name_str.c_str(), (rapidjson::SizeType)key_name_str.length(), alloc);
        temp.AddMember(str, arr, alloc);
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        temp.Accept(writer);

    } catch(const char *message) {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
        {
            std::string msg = std::string(message) + std::string("key - ") + std::string(key_name);
            this->errorHandler(__FUNCTION__, msg, ErrorCode::GeneralError);
		}
#else
		this->errorHandler(ErrorCode::GeneralError);
#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    }

    this->pEtal = &this->curr_obj;
}

/******************************************************************************
 *  @brief AddObjectsArray
 *
 *  @param key_name
 *  @param sizeArray
 *  @param Parameter description.
 *          @arg PARAM1_POSSIBLE_VALUE: value 1 description
 *
 *  @retval description
 *****************************************************************************/
template<typename T0>
void JsonCpp::AddObjectsArray(T0 &key_name, int sizeArray) {
   
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
    
    try {

        if(sizeArray > 0) {
            std::string key_name_str = key_name;
            rapidjson::Value& temp = *this->pEtal;
            rapidjson::Document::AllocatorType& alloc = this->document.GetAllocator();
            rapidjson::Value arr(rapidjson::kArrayType);
            rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();

            for (int i = 0; i < sizeArray; i++) {
                rapidjson::Value newObject(rapidjson::kObjectType);
                arr.PushBack(newObject, alloc);
            }
            
            rapidjson::Value str;
            str.SetString(key_name_str.c_str(), (rapidjson::SizeType)key_name_str.length(), alloc);
            temp.AddMember(str, arr, alloc);
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            temp.Accept(writer);

        } else {
            throw ("Array size mest be > 1");
        }
    } catch(const char* message) {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
        {
            std::string msg = std::string(message) + std::string("key - ") + std::string(key_name);
            this->errorHandler(__FUNCTION__, msg, ErrorCode::GeneralError);
        }
#else
		this->errorHandler(ErrorCode::GeneralError);
#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
    
    }

    this->pEtal = &this->curr_obj;
}

/******************************************************************************
 *  @brief  AddObject
 *
 *  @param  key_name
 *  @param  Parameter description.
 *          @arg PARAM1_POSSIBLE_VALUE: value 1 description
 *
 *  @retval description
 *****************************************************************************/
template <typename T>
void JsonCpp::AddObject(T &key_name) {
    
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
    
    try {

        std::string key_name_str = key_name;
        rapidjson::Value& temp = *this->pEtal;
        rapidjson::Document::AllocatorType& alloc = this->document.GetAllocator();
        rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();

        rapidjson::Value anonym(rapidjson::kObjectType);
        rapidjson::Value str;
        str.SetString(key_name_str.c_str(), (rapidjson::SizeType)key_name_str.length(), alloc);
        temp.AddMember(str, anonym, alloc);

    } catch(const char* message) {

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER 

        std::string msg = std::string(message) + std::string("key - ") + std::string(key_name);
        this->errorHandler(__FUNCTION__, msg, ErrorCode::RapidJsonError);

#else
		this->errorHandler(ErrorCode::RapidJsonError);

#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
       
    }

    this->pEtal = &this->curr_obj;
}

/*-- Exported functions -----------------------------------------------------*/

#endif // JECTJSON_H
/*-- EOF --------------------------------------------------------------------*/
