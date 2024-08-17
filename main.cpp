#include <windows.h>

#include <string>
#include <vector>
#include <iostream>
#include <format>

using std::cout;

using std::endl;
using std::string;
using std::string_view;
using std::vector;

using str = std::string; // NOTE: all std::string are utf-8 encoded
using wstr = std::wstring;
using str_cref = std::string const&;
using wstr_cref = std::wstring const&;

template<typename T>
using vec = vector<T>;

using namespace std::string_literals;
using namespace std::string_view_literals;


str to_UTF8(wstr_cref wide_str)
{
    int size = WideCharToMultiByte(
        CP_UTF8, 0, 
        wide_str.data(), wide_str.size(),
        nullptr, 0, 
        nullptr, nullptr);

    if (size == 0)
        return "";

    auto utf8_str = std::string(size, '\0');

    WideCharToMultiByte(
        CP_UTF8, 0, 
        wide_str.data(), wide_str.size(),
        utf8_str.data(), utf8_str.size(), 
        nullptr, nullptr);

    return utf8_str;
}

wstr to_wide(str_cref utf8_str)
{
    int size = MultiByteToWideChar(
        CP_UTF8, 0, 
        utf8_str.data(), utf8_str.size(), 
        nullptr, 0);

    if (size == 0)
        return L"";

    wstr wide_str(size, L'\0');

    MultiByteToWideChar(
        CP_UTF8, 0, 
        utf8_str.data(), utf8_str.size(), 
        wide_str.data(), wide_str.size());

    return wide_str;
}


str last_error_as_string(DWORD last_error)
{
    auto constexpr buffer_count = 1024;
    WCHAR buffer[buffer_count] {};

    DWORD size = FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_MAX_WIDTH_MASK,
        NULL,
        last_error,
        0,
        (wchar_t*)&buffer,
        buffer_count,
        NULL);

    return to_UTF8(wstr(buffer, size));
}

bool is_user_admin()
{
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup {};

    BOOL success = AllocateAndInitializeSid(
        &NtAuthority,
        2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &AdministratorsGroup);

    if (not success)
    {
        FreeSid(AdministratorsGroup);
        throw std::format("[ERROR] Cannot allocate SID: {}",
                          last_error_as_string(GetLastError()));
    }

    BOOL is_member = false;

    success = CheckTokenMembership(NULL, AdministratorsGroup, &is_member);

    if (not success)
    {
        FreeSid(AdministratorsGroup);
        throw std::format("[ERROR] CheckTokenMembership failed: {}",
                          last_error_as_string(GetLastError()));
    }

    FreeSid(AdministratorsGroup);

    return (is_member > 0);
}

int main()
{
    try
    {
        auto res = is_user_admin();

        if (res)
        {
            cout << "YES" << endl;
            return 1;
        }
        else
        {
            cout << "NO" << endl;
            return 0;
        }
    }
    catch(const str_cref e)
    {
        std::cerr << e << '\n';
    }
    
    return -1;
}