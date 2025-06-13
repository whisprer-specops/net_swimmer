#ifndef BLOB_TRANSPORT_H
#define BLOB_TRANSPORT_H

#include <string>
#include <vector>

std::string encrypt_blob(const std::string& plain_text);
std::string decrypt_blob(const std::string& encrypted_blob);

bool send_via_tor(const std::string& encrypted_blob);
std::string receive_from_tor();

#endif // BLOB_TRANSPORT_H

#endif // BLOB_TRANSPORT_H
