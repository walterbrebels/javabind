/**
 * javabind: effective C++ and Java interoperability
 * @see https://github.com/hunyadi/javabind
 *
 * Copyright (c) 2024 Levente Hunyadi
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <javabind/javabind.hpp>
#include <charconv>
#include <vector>

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::pair<K, V>& pair)
{
    return os << pair.first << ": " << pair.second;
}

template <typename L>
std::ostream& write_bracketed_list(std::ostream& os, const L& list, char left, char right)
{
    os << left;
    if (!list.empty()) {
        auto&& it = list.begin();
        os << *it;

        for (++it; it != list.end(); ++it) {
            os << ", " << *it;
        }
    }
    os << right;
    return os;
}

template <typename L>
std::ostream& write_list(std::ostream& os, const L& vec)
{
    return write_bracketed_list(os, vec, '[', ']');
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& list)
{
    return write_list(os, list);
}

template <typename S>
std::ostream& write_set(std::ostream& os, const S& set)
{
    return write_bracketed_list(os, set, '{', '}');
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& set)
{
    return write_set(os, set);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::unordered_set<T>& set)
{
    return write_set(os, set);
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& set)
{
    return write_set(os, set);
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<K, V>& set)
{
    return write_set(os, set);
}

struct Rectangle
{
    Rectangle() = default;
    Rectangle(double width, double height)
        : width(width)
        , height(height)
    {}

    double width = 0.0;
    double height = 0.0;
};

std::ostream& operator<<(std::ostream& os, const Rectangle& rect)
{
    return os << "{" << rect.width << ", " << rect.height << "}";
}

struct PrimitiveRecord
{
    int8_t byte_value;
    char16_t char_value;
    int16_t short_value;
    int32_t int_value;
    int64_t long_value;
    float float_value;
    double double_value;
};

struct Sample
{
    static void returns_void()
    {
        JAVA_OUTPUT << "returns_void()" << std::endl;
    }

    static bool returns_bool()
    {
        JAVA_OUTPUT << "returns_bool()" << std::endl;
        return true;
    }

    static int32_t returns_int()
    {
        JAVA_OUTPUT << "returns_int()" << std::endl;
        return 82;
    }

    int32_t value() const
    {
        return _value;
    }

    void operator+=(int32_t val)
    {
        _value += val;
    }

private:
    int32_t _value = 0;
};

static void returns_void()
{
    JAVA_OUTPUT << "returns_void()" << std::endl;
}

struct StaticSample
{
    static bool returns_bool()
    {
        JAVA_OUTPUT << "returns_bool()" << std::endl;
        return true;
    }

    static int32_t returns_int()
    {
        JAVA_OUTPUT << "returns_int()" << std::endl;
        return 82;
    }

    static std::string returns_string()
    {
        JAVA_OUTPUT << "returns_string()" << std::endl;
        return "a sample string";
    }

    template <typename T>
    static T pass_value(T value)
    {
        JAVA_OUTPUT << "pass_value(" << value << ")" << std::endl;
        return value;
    }

    static std::string pass_string(const std::string& value)
    {
        JAVA_OUTPUT << "pass_string(" << value << ")" << std::endl;
        return value;
    }

    static std::string pass_utf8_string(const std::string_view& value)
    {
        JAVA_OUTPUT << "pass_utf8_string(" << value << ")" << std::endl;
        return std::string(value);
    }

    static void pass_utf16_string(const std::u16string_view& value)
    {
        JAVA_OUTPUT << "pass_utf16_string(len = " << value.size() << ")" << std::endl;
    }

    template <typename T>
    static javabind::boxed<T> pass_boxed(javabind::boxed<T> value)
    {
        JAVA_OUTPUT << "pass_boxed(" << value << ")" << std::endl;
        return value;
    }

    template <typename T>
    static std::vector<T> pass_array(const std::vector<T>& values)
    {
        JAVA_OUTPUT << "pass_array(" << values << ")" << std::endl;
        return std::vector<T>(values.begin(), values.end());
    }

    template <typename T>
    static std::vector<T> pass_array_view(const std::basic_string_view<T>& values)
    {
        auto result = std::vector<T>(values.begin(), values.end());
        JAVA_OUTPUT << "pass_array_view(" << result << ")" << std::endl;
        return result;
    }

    static std::string pass_function(const std::string& str, const std::function<std::string(std::string)>& fn)
    {
        JAVA_OUTPUT << "pass_function(" << str << ")" << std::endl;
        return str + " -> " + fn(str);
    }

    static std::function<std::string(std::string)> returns_function(const std::string& search, const std::string& replace)
    {
        return
            [search, replace](const std::string& str) -> std::string
            {
                std::string res;
                std::size_t index = 0;
                while (true) {
                    std::size_t pos = str.find(search, index);
                    if (pos == std::string::npos) {
                        res.append(str.substr(index));
                        break;
                    }
                    res.append(str.substr(index, pos - index));
                    res.append(replace);
                    index = pos + search.size();
                }
                return res;
            };
    }

    template <typename T>
    static void apply_consumer(T val, const std::function<void(T)>& fn)
    {
        JAVA_OUTPUT << "apply_consumer(" << val << ")" << std::endl;
        fn(val);
    }

    template <typename T>
    static bool apply_predicate(T val, const std::function<bool(T)>& fn)
    {
        JAVA_OUTPUT << "apply_predicate(" << val << ")" << std::endl;
        return fn(val);
    }

    template <typename T>
    static std::string apply_to_string_function(T val, const std::function<std::string(T)>& fn)
    {
        JAVA_OUTPUT << "apply_to_string_function(" << val << ")" << std::endl;
        return fn(val);
    }

    template <typename R>
    static R apply_from_string_function(const std::string& val, const std::function<R(std::string)>& fn)
    {
        JAVA_OUTPUT << "apply_from_string_function(" << val << ")" << std::endl;
        return fn(val);
    }

    template <typename T>
    static std::function<std::string(T)> get_to_string_function()
    {
        return
            [](T val)
            {
                std::array<char, 64> buf;
                std::string str;
                auto res = std::to_chars(buf.data(), buf.data() + buf.size(), val);
                if (res.ec != std::errc{}) {
                    throw std::runtime_error("to_chars");
                }
                return std::string(buf.data(), res.ptr);
            };
    }

    template <typename R>
    static std::function<R(std::string)> get_from_string_function()
    {
        return
            [](const std::string& str)
            {
                R val;
                auto res = std::from_chars(str.data(), str.data() + str.size(), val);
                if (res.ec != std::errc() || res.ptr != str.data() + str.size()) {
                    throw std::runtime_error("from_chars");
                }
                return val;
            };
    }

    static std::function<double(std::string)> get_string_to_double_function()
    {
        return
            [](const std::string& str)
            {
                std::istringstream iss(str);
                double val;
                iss >> val;
                if (iss.fail() || !iss.eof()) {
                    throw std::runtime_error("istringstream");
                }
                return val;
            };
    }

    template <typename T>
    static std::function<void(T)> get_consumer()
    {
        return
            [](T val)
            {
                JAVA_OUTPUT << "consume: " << val << std::endl;
            };
    }

    static Rectangle pass_record(const Rectangle& rect)
    {
        JAVA_OUTPUT << "pass_record(" << rect << ")" << std::endl;
        return Rectangle(2 * rect.width, 2 * rect.height);
    }

    static PrimitiveRecord transform_record(const PrimitiveRecord& rec)
    {
        return PrimitiveRecord{
            static_cast<int8_t>(rec.byte_value * 2),
            rec.char_value,
            static_cast<int16_t>(rec.short_value * 2),
            rec.int_value * 2,
            rec.long_value * 2l,
            rec.float_value * 2.0f,
            rec.double_value * 2.0
        };
    }

    template <typename C>
    static C pass_collection(const C& collection)
    {
        JAVA_OUTPUT << "pass_collection(" << collection << ")" << std::endl;
        return C(collection.begin(), collection.end());
    }
};

struct Residence
{
    std::string country;
    std::string city;
};

class Person
{
    std::string name;
    Residence residence;
    std::vector<Person> children;

public:
    Person() = default;
    Person(const std::string& n) : name(n) {}
    Person(const std::string& n, const Residence& r) : name(n), residence(r) {}

    const std::string& get_name() const { return name; }
    void set_name(const std::string& n) { name = n; }

    Residence get_residence() const { return residence; }
    void set_residence(const Residence& r) { residence = r; }

    const std::vector<Person>& get_children() const { return children; }
    void set_children(std::vector<Person> c) { children = std::move(c); }
};

std::ostream& operator<<(std::ostream& os, const Person& person)
{
    return os << "{" << person.get_name() << "}";
}

DECLARE_NATIVE_CLASS(Sample, "hu.info.hunyadi.test.Sample");
DECLARE_RECORD_CLASS(Rectangle, "hu.info.hunyadi.test.Rectangle");
DECLARE_RECORD_CLASS(PrimitiveRecord, "hu.info.hunyadi.test.PrimitiveRecord");
DECLARE_STATIC_CLASS(StaticSample, "hu.info.hunyadi.test.StaticSample");

DECLARE_NATIVE_CLASS(Person, "hu.info.hunyadi.test.Person");
DECLARE_RECORD_CLASS(Residence, "hu.info.hunyadi.test.Residence");

JAVA_EXTENSION_MODULE()
{
    using namespace javabind;

    record_class<Rectangle>()
        .field<&Rectangle::width>("width")
        .field<&Rectangle::height>("height")
        ;

    record_class<PrimitiveRecord>()
        .field<&PrimitiveRecord::byte_value>("byte_value")
        .field<&PrimitiveRecord::char_value>("char_value")
        .field<&PrimitiveRecord::short_value>("short_value")
        .field<&PrimitiveRecord::int_value>("int_value")
        .field<&PrimitiveRecord::long_value>("long_value")
        .field<&PrimitiveRecord::float_value>("float_value")
        .field<&PrimitiveRecord::double_value>("double_value")
        ;

    native_class<Sample>()
        .constructor<Sample()>("create")
        .function<Sample::returns_void>("returns_void")
        .function<Sample::returns_bool>("returns_bool")
        .function<Sample::returns_int>("returns_int")
        .function<&Sample::value>("value")
        .function < &Sample::operator+=>("add")
        ;

    static_class<StaticSample>()
        // fundamental types and simple well-known types as return values
        .function<returns_void>("returns_void")
        .function<StaticSample::returns_bool>("returns_bool")
        .function<StaticSample::returns_int>("returns_int")
        .function<StaticSample::returns_string>("returns_string")

        // fundamental types and simple well-known types as arguments
        .function<StaticSample::pass_value<bool>>("pass_bool")
        .function<StaticSample::pass_value<int8_t>>("pass_byte")
        .function<StaticSample::pass_value<char16_t>>("pass_char")
        .function<StaticSample::pass_value<int16_t>>("pass_short")
        .function<StaticSample::pass_value<int32_t>>("pass_int")
        .function<StaticSample::pass_value<int64_t>>("pass_long")
        .function<StaticSample::pass_value<float>>("pass_float")
        .function<StaticSample::pass_value<double>>("pass_double")
        .function<StaticSample::pass_string>("pass_string")
        .function<StaticSample::pass_utf8_string>("pass_utf8_string")
        .function<StaticSample::pass_utf16_string>("pass_utf16_string")

        // boxing and unboxing
        .function<StaticSample::pass_boxed<bool>>("pass_boxed_boolean")
        .function<StaticSample::pass_boxed<int32_t>>("pass_boxed_integer")
        .function<StaticSample::pass_boxed<int64_t>>("pass_boxed_long")
        .function<StaticSample::pass_boxed<double>>("pass_boxed_double")

        // arrays as arguments and return values
        .function<StaticSample::pass_array<bool>>("pass_bool_array")
        .function<StaticSample::pass_array<int8_t>>("pass_byte_array")
        .function<StaticSample::pass_array<char16_t>>("pass_char_array")
        .function<StaticSample::pass_array<int16_t>>("pass_short_array")
        .function<StaticSample::pass_array<int32_t>>("pass_int_array")
        .function<StaticSample::pass_array<int64_t>>("pass_long_array")
        .function<StaticSample::pass_array<float>>("pass_float_array")
        .function<StaticSample::pass_array<double>>("pass_double_array")
        .function<StaticSample::pass_array_view<bool>>("pass_bool_array_view")
        .function<StaticSample::pass_array_view<int8_t>>("pass_byte_array_view")
        .function<StaticSample::pass_array_view<int16_t>>("pass_short_array_view")
        .function<StaticSample::pass_array_view<int32_t>>("pass_int_array_view")
        .function<StaticSample::pass_array_view<int64_t>>("pass_long_array_view")
        .function<StaticSample::pass_array_view<float>>("pass_float_array_view")
        .function<StaticSample::pass_array_view<double>>("pass_double_array_view")

        // functional interface
        .function<StaticSample::pass_function>("pass_function")
        .function<StaticSample::returns_function>("returns_function")
        .function<StaticSample::apply_consumer<int32_t>>("apply_int_consumer")
        .function<StaticSample::apply_consumer<int64_t>>("apply_long_consumer")
        .function<StaticSample::apply_consumer<double>>("apply_double_consumer")
        .function<StaticSample::apply_consumer<std::string>>("apply_string_consumer")
        .function<StaticSample::apply_predicate<int32_t>>("apply_int_predicate")
        .function<StaticSample::apply_predicate<int64_t>>("apply_long_predicate")
        .function<StaticSample::apply_predicate<double>>("apply_double_predicate")
        .function<StaticSample::apply_predicate<std::string>>("apply_string_predicate")
        .function<StaticSample::apply_to_string_function<int32_t>>("apply_int_to_string_function")
        .function<StaticSample::apply_to_string_function<int64_t>>("apply_long_to_string_function")
        .function<StaticSample::apply_to_string_function<double>>("apply_double_to_string_function")
        .function<StaticSample::apply_from_string_function<int32_t>>("apply_string_to_int_function")
        .function<StaticSample::apply_from_string_function<int64_t>>("apply_string_to_long_function")
        .function<StaticSample::apply_from_string_function<double>>("apply_string_to_double_function")
        .function<StaticSample::get_to_string_function<int32_t>>("get_int_to_string_function")
        .function<StaticSample::get_to_string_function<int64_t>>("get_long_to_string_function")
        .function<StaticSample::get_to_string_function<double>>("get_double_to_string_function")
        .function<StaticSample::get_from_string_function<int32_t>>("get_string_to_int_function")
        .function<StaticSample::get_from_string_function<int64_t>>("get_string_to_long_function")
        .function<StaticSample::get_string_to_double_function>("get_string_to_double_function")
        .function<StaticSample::get_consumer<std::string_view>>("get_string_consumer")
        .function<StaticSample::get_consumer<int32_t>>("get_int_consumer")
        .function<StaticSample::get_consumer<int64_t>>("get_long_consumer")
        .function<StaticSample::get_consumer<double>>("get_double_consumer")
        .function<StaticSample::get_consumer<Person&>>("get_person_ref_consumer")
        .function<StaticSample::get_consumer<const Person&>>("get_person_const_ref_consumer")

        // record class
        .function<StaticSample::pass_record>("pass_record")
        .function<StaticSample::transform_record>("transform_record")

        // collection types
        .function<StaticSample::pass_collection<std::vector<Rectangle>>>("pass_list")
        .function<StaticSample::pass_collection<std::set<std::string>>>("pass_ordered_set")
        .function<StaticSample::pass_collection<std::unordered_set<std::string>>>("pass_unordered_set")
        .function<StaticSample::pass_collection<std::map<std::string, Rectangle>>>("pass_ordered_map")
        .function<StaticSample::pass_collection<std::unordered_map<std::string, Rectangle>>>("pass_unordered_map")
        ;


    native_class<Person>()
        .constructor<Person(std::string)>("create")
        .constructor<Person(std::string, Residence)>("create")
        .function<&Person::get_name>("getName")
        .function<&Person::set_name>("setName")
        .function<&Person::get_residence>("getResidence")
        .function<&Person::set_residence>("setResidence")
        .function<&Person::get_children>("getChildren")
        .function<&Person::set_children>("setChildren")
        ;

    record_class<Residence>()
        .field<&Residence::country>("country")
        .field<&Residence::city>("city")
        ;

    print_registered_bindings();
}
