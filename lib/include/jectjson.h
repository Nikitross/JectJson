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

    typedef std::function<void(const char* message, const char* details)> ErrorCallback;
	typedef std::function<void(const std::string& modul_name,const std::string& message,const std::string& details)> ErrorCallback2;
    
	typedef struct {
        ErrorCallback onError;
		ErrorCallback2 onError2;
    } userCallbacksInfo;

public:

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
    uint32_t GetSizeArray(const char* key_val);

    std::string GetElementType(const rapidjson::Value& val);

    bool SetSringDataBuffer(const char* DataStrBuff);
    std::string GetStringDataBuffer();

    void PrintIt();
    void RemoveKey(const char* key_val);
    bool RegisterCallback_onError(const ErrorCallback& callbackFunc);
	bool RegisterCallback_onError(const ErrorCallback2& callbackFunc);

    template <typename TypeOutData>
    TypeOutData GetValue(const char* key_val);
    template <typename TypeOutData>
    TypeOutData GetValue(const char* key_val, uint32_t index_value);

    template <typename TypeData>
    void ChangeValue(const char* key_val, TypeData data);
    template <typename TypeData>
    void ChangeArray(const char* key_val, TypeData* type_data, uint32_t size_array);

    template <typename TypeValue0, typename TypeValue1>
    void AddValue(TypeValue0 &key_num0, TypeValue1 key_num1);
    template <typename T0, typename T1>
    void AddArray(T0 &key_name, T1 array_value, int sizeArray);
    template<typename T0>
    void AddObjectsArray(T0 &key_name, int sizeArray);
    template <typename T>
    void AddObject(T &key_name);

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

    bool has_open_json;

	std::string GetArray(const char* key_val);
    void RemoveKey(const char* key_val, uint32_t size_data);
    
    void errorHandler(const char* message, const char* details);
	void errorHandler(const std::string& modul_name,const std::string& message,const std::string& details);
    bool IsValidJson(const std::string& json_file);
    
    template<typename InputType, typename OutpuType>
    constexpr inline void getTypeError() {
        if (!::std::is_same<InputType, OutpuType>::value) {
            this->errorHandler("ERROR, USER TYPE IS NOT REAL TYPE OF VALUE!", " RETURN DEFAULT VALUE");
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
TypeOutData JsonCpp::GetValue(const char* key_val) {

    if (!this->has_open_json) {
        this->errorHandler("function GetValue: JSON is not open!:", key_val); 
        return TypeOutData();
    }

    std::string key_name = key_val;
    TypeOutData out = TypeOutData();
    TypeOutData out_type = TypeOutData();
    rapidjson::Value &temp = *this->pEtal;
    rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();

    if (current_iterator == temp.MemberEnd()) {
        this->errorHandler("function GetValue: NOT FOUND KEY:", key_name.c_str());
    }

    for(; current_iterator != temp.MemberEnd(); ++current_iterator) {
        if(key_name.compare(current_iterator->name.GetString()) == 0) {
            if(current_iterator->value.IsBool()) {
                getTypeError<TypeOutData, bool>();
                out = getT<TypeOutData>(out_type, current_iterator->value.GetBool());
            } else if(current_iterator->value.IsInt()) {
                getTypeError<TypeOutData, int>();
                out = getT<TypeOutData>(out_type, current_iterator->value.GetInt());
            } else if(current_iterator->value.IsFloat()) {
                getTypeError<TypeOutData, float>();
                out = getT<TypeOutData>(out_type, current_iterator->value.GetFloat());
            } else if(current_iterator->value.IsString()) {
                getTypeError<TypeOutData, std::string>();
                out = getT<TypeOutData>(out_type, static_cast<std::string>(current_iterator->value.GetString()));
            } else if(current_iterator->value.IsDouble()) {
                getTypeError<TypeOutData, double>();
                out = getT<TypeOutData>(out_type, current_iterator->value.GetDouble());
            } else {
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
TypeOutData JsonCpp::GetValue(const char* key_val, uint32_t index_value) {

    if (!this->has_open_json) {
        this->errorHandler("function GetValue: JSON is not open!:", key_val);
        return TypeOutData();
    }

    std::string key_name = key_val;
    TypeOutData out = TypeOutData();
    TypeOutData out_type = TypeOutData();
    rapidjson::Value &temp = *this->pEtal;
    rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();

    if (current_iterator == temp.MemberEnd()) {
        this->errorHandler("function GetValue: NOT FOUND KEY:", key_name.c_str());
    }

    bool exist_key = false;

    for(; current_iterator != temp.MemberEnd(); ++current_iterator) {
        if(key_name.compare(current_iterator->name.GetString()) == 0) {
            exist_key = true;
            if(current_iterator->value.IsArray()) {
                if(current_iterator->value[index_value].IsBool()) {
                    getTypeError<TypeOutData, bool>();
                    out = getT<TypeOutData>(out_type, current_iterator->value[index_value].GetBool());
                } else if(current_iterator->value[index_value].IsInt()) {
                    getTypeError<TypeOutData, int>();
                    out = getT<TypeOutData>(out_type, current_iterator->value[index_value].GetInt());
                } else if(current_iterator->value[index_value].IsFloat()) {
                    getTypeError<TypeOutData, float>();
                    out = getT<TypeOutData>(out_type, current_iterator->value[index_value].GetFloat());
                } else if(current_iterator->value[index_value].IsString()) {
                    getTypeError<TypeOutData, std::string>();
                    out = getT<TypeOutData>(out_type, static_cast<std::string>(current_iterator->value[index_value].GetString()));
                } else if(current_iterator->value[index_value].IsDouble()) {
                    getTypeError<TypeOutData, double>();
                    out = getT<TypeOutData>(out_type, current_iterator->value[index_value].GetDouble());
                } else {
                    return TypeOutData();
                }
            } else {
                out = TypeOutData();
            }
            break;
        }
    }

    if (!exist_key) {
        this->errorHandler("function GetValue: NOT FOUND KEY:", key_name.c_str());
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
void JsonCpp::ChangeValue(const char* key_val, TypeData data) {

    if (!this->has_open_json) {
        this->errorHandler("function ChangeValue: JSON is not open!:", key_val);
        return;
    }

    std::string key_name = key_val;
    rapidjson::Value &temp = *this->pEtal;
    rapidjson::Document::AllocatorType &alloc = this->document.GetAllocator();
    rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();

    if (current_iterator == temp.MemberEnd()) {
        this->errorHandler("function ChangeValue: NOT FOUND KEY:", key_name.c_str());
    }

    bool exist_key = false;

    for(; current_iterator != temp.MemberEnd(); ++current_iterator) {
        if(key_name.compare(current_iterator->name.GetString()) == 0) {
            exist_key = true;
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

    if (!exist_key) {
        this->errorHandler("function GetValue: NOT FOUND KEY:", key_name.c_str());
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
void JsonCpp::ChangeArray(const char* key_val, TypeData* type_data, uint32_t size_array) {

    if (!this->has_open_json) {
        this->errorHandler("function ChangeArray: JSON is not open!:", "");
        return;
    }

    rapidjson::Value &temp = *this->pEtal;
    this->RemoveKey(key_val);
    this->pEtal = &temp;
    this->AddArray(key_val, type_data, size_array);
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
void JsonCpp::AddValue(TypeValue0 &key_num0, TypeValue1 key_num1) {

    if (!this->has_open_json) {
        this->errorHandler("function GetValue: JSON is not open!:", "");
        return;
    }

    try {
        std::string temp_key_name = key_num0;
        rapidjson::Value &temp = *this->pEtal;
        rapidjson::Document::AllocatorType &alloc = this->document.GetAllocator();
        rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();
        rapidjson::Value str;
        str.SetString(temp_key_name.c_str(), (rapidjson::SizeType)temp_key_name.length(), alloc);
        bool keyAlreadyExist = false;

        for(; current_iterator != temp.MemberEnd(); ++current_iterator) {
            if(temp_key_name.compare(current_iterator->name.GetString()) == 0) {
                keyAlreadyExist = true;
            }
        }

        if(!keyAlreadyExist) {
            rapidjson::Value val;

            if(typeid(key_num1) == typeid(const char*)) {
                val.SetString(rapidjson::StringRef(setT<char>("char", key_num1)));
                temp.AddMember(str, val, alloc);
            } else if(typeid(key_num1) == typeid(std::string)) {
                val.SetString(setT<std::string>(std::string(), key_num1).c_str(), static_cast<int>(setT<std::string>(std::string(), key_num1).length()), alloc);
                temp.AddMember(str, val, alloc);
            } else if(typeid(key_num1) == typeid(int)) {
                val.SetInt(setT<int>(int(), key_num1));
                temp.AddMember(str, val, alloc);
            } else if(typeid(key_num1) == typeid(double)) {
                val.SetDouble(setT<double>(double(), key_num1));
                temp.AddMember(str, val, alloc);
            } else if(typeid(key_num1) == typeid(float)) {
                val.SetFloat(setT<float>(float(), key_num1));
                temp.AddMember(str, val, alloc);
            } else if(typeid(key_num1) == typeid(bool)) {
                val.SetBool(setT<bool>(bool(), key_num1));
                temp.AddMember(str, val, alloc);
            }

            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            temp.Accept(writer);
        }

    } catch(const char *message) {
        this->errorHandler("Func AddValue", message);
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
    
    if (!this->has_open_json) {
        this->errorHandler("function AddArray: JSON is not open!:", " ");
        return;
    }
    
    try {

        std::string temp_key_name = key_name;
        rapidjson::Value &temp = *this->pEtal;
        rapidjson::Document::AllocatorType &alloc = this->document.GetAllocator();
        rapidjson::Value arr(rapidjson::kArrayType);
        rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();
        bool keyAlreadyExist = false;

        for(; current_iterator != temp.MemberEnd(); ++current_iterator) {
            if(temp_key_name.compare(current_iterator->name.GetString()) == 0) {
                keyAlreadyExist = true;
            }
        }

        if(!keyAlreadyExist) {
            if(typeid(array_value) == typeid(std::string*)) {
                rapidjson::Value strVal;
                std::string st;
                for(int i = 0; i < sizeArray; i++) {
                    st = setT<std::string>(std::string(), array_value[i]);
                    strVal.SetString(st.c_str(), (rapidjson::SizeType)st.length(), alloc);
                    arr.PushBack(strVal, alloc);
                }

            } else if(typeid(array_value) == typeid(int*) || typeid(array_value) == typeid(uint8_t*) || typeid(array_value) == typeid(uint32_t*)) {
                for(int i = 0; i < sizeArray; i++) {
                    arr.PushBack(setT<int>(int(), array_value[i]), alloc);
                }
            } else if(typeid(array_value) == typeid(float*)) {
                for(int i = 0; i < sizeArray; i++) {
                    arr.PushBack(setT<float>(float(), array_value[i]), alloc);
                }
            } else if(typeid(array_value) == typeid(double*)) {
                for(int i = 0; i < sizeArray; i++) {
                    arr.PushBack(setT<double>(double(), array_value[i]), alloc);
                }
            } else if(typeid(array_value) == typeid(bool*)) {
                for(int i = 0; i < sizeArray; i++) {
                    arr.PushBack(setT<bool>(bool(), array_value[i]), alloc);
                }
            } else {
                throw "type input";
            }

            rapidjson::Value str;
            str.SetString(temp_key_name.c_str(), (rapidjson::SizeType)temp_key_name.length(), alloc);
            temp.AddMember(str, arr, alloc);
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            temp.Accept(writer);
        }

    } catch(const char *message) {
        this->errorHandler("Func AddArray", message);
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
   
    if (!this->has_open_json) {
        this->errorHandler("function AddObjectsArray: JSON is not open!:", ""); 
        return;
    }
    
    try {
        if(sizeArray > 0) {
            std::string temp_key_name = key_name;
            rapidjson::Value& temp = *this->pEtal;
            rapidjson::Document::AllocatorType& alloc = this->document.GetAllocator();
            rapidjson::Value arr(rapidjson::kArrayType);
            rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();
            bool keyAlreadyExist = false;

            for(; current_iterator != temp.MemberEnd(); ++current_iterator) {
                if(temp_key_name.compare(current_iterator->name.GetString()) == 0) {
                    keyAlreadyExist = true;
                }
            }

            if(!keyAlreadyExist) {
                for(int i = 0; i < sizeArray; i++) {
                    rapidjson::Value newObject(rapidjson::kObjectType);
                    arr.PushBack(newObject, alloc);
                }
                rapidjson::Value str;
                str.SetString(temp_key_name.c_str(), (rapidjson::SizeType)temp_key_name.length(), alloc);
                temp.AddMember(str, arr, alloc);
                rapidjson::StringBuffer buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                temp.Accept(writer);
            }

        } else {
            throw ("Array size mest be > 1");
        }
    } catch(const char* message) {
        this->errorHandler("Func AddObjectsArray", message);
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
    
    if (!this->has_open_json) {
        this->errorHandler("function AddObject: JSON is not open!:", "");
        return;
    }
    
    try {

        std::string temp_key_name = key_name;
        rapidjson::Value& temp = *this->pEtal;
        rapidjson::Document::AllocatorType& alloc = this->document.GetAllocator();
        rapidjson::Value::ConstMemberIterator current_iterator = temp.MemberBegin();
        bool keyAlreadyExist = false;

        for(; current_iterator != temp.MemberEnd(); ++current_iterator) {
            if(temp_key_name.compare(current_iterator->name.GetString()) == 0) {
                keyAlreadyExist = true;
            }
        }

        if(!keyAlreadyExist) {
            rapidjson::Value anonym(rapidjson::kObjectType);
            rapidjson::Value str;
            str.SetString(temp_key_name.c_str(), (rapidjson::SizeType)temp_key_name.length(), alloc);
            temp.AddMember(str, anonym, alloc);
        }

    } catch(const char* message) {
        this->errorHandler("Func AddObject", message);
    }

    this->pEtal = &this->curr_obj;
}

/*-- Exported functions -----------------------------------------------------*/
#endif // JECTJSON_H
/*-- EOF --------------------------------------------------------------------*/
