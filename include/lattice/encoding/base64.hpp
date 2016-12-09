//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Base64 encoding and decoding.
 *
 *  The original methods were modified from Inge Eivind Henriksen,
 *  provided under an MIT license, and can be found [here](1).
 *
 *  1. https://svn.code.sf.net/p/tmplusplus/code/trunk/COPYING
 */

#pragma once

#include <string>
#include <vector>


namespace lattice
{
namespace base64
{
// CONSTANTS
// ---------

extern const size_t INPUT_INTERVAL;
extern const size_t OUTPUT_INTERVAL;
extern const char ENCODING[65];
extern const char DECODING[256];

// FUNCTIONS
// ---------


/** \brief Create buffer padded to a fixed size.
 *
 *  \warning The allocated buffer must be cleaned up by the caller.
 *
 *  Create a buffer between an iterator range padded to a fixed size
 *  with padding characters.
 */
template <typename Iter>
std::string paddedBuffer(Iter start,
    Iter end,
    size_t size,
    char pad = '\0')
{
    // reserve
    std::string buffer;
    buffer.reserve(size);

    // fill
    size_t index = 0;
    for (; start < end; ++index, ++start) {
        buffer.push_back(*start);
    }
    for (;  index < size; ++index) {
        buffer.push_back(pad);
    }
    return buffer;
}


/** \brief Calculate non-padded output bytes from input bytes.
 *
 *  \param byteCount    Number of bytes in input
 *  @return             Total number of encoded bytes in digest
 */
size_t encodedByteCount(const size_t byteCount);

/** \brief Estimate encoded length of message.
 *
 *  \param messageLength        Length of message
 *  \return                     Length of padded, base64-encoded message
 */
size_t encodedSize(const size_t messageLength);

/** \brief Estimate decoded length of base64-encoded message.
 *
 *  \param messageLength        Length of padded, base64-encoded message
 *  \return                     Length of decoded message
 */
size_t decodedSize(const size_t messageLength);

/** \brief Calculate exact length of base64-decoded message.
 *
 *  \param start                Iterator at start of input buffer
 *  \param end                  Iterator at end of input buffer
 *  \return                     Length of decoded message
 */
template <typename Iter>
size_t decodedSize(const Iter start,
    Iter end)
{
    size_t messageLength = end - start;
    while(*--end == '=') {
        messageLength--;
    }

    return decodedSize(messageLength);
}


/** \brief Encode 3 bytes to base64.
 *
 *  \param output           Iterator at start of output buffer
 *  \param input            Iterator at start of input buffer
 */
template <
    typename Container,
    typename Iter
>
void encodeMessage(Container &output,
    Iter &input)
{
    char *buffer = new char[INPUT_INTERVAL];
    // Fails, needs to be null-intiialized
    for (size_t i = 0; i < INPUT_INTERVAL; ++i, ++input) {
        buffer[i] = *input;
    }
    // First: 11111100
    output.push_back(ENCODING[(buffer[0] & 0xfc) >> 2]);
    // First: 00000011, Second: 11110000
    output.push_back(ENCODING[((buffer[0] & 0x03) << 4) + ((buffer[1] & 0xf0) >> 4)]);
    // First: 00001111, Second: 11000000
    output.push_back(ENCODING[((buffer[1] & 0x0f) << 2) + ((buffer[2] & 0xc0) >> 6)]);
    // First: 00111111
    output.push_back(ENCODING[buffer[2] & 0x3f]);

    delete[] buffer;
}


/** \brief Decode 3 bytes from base64.
 *
 *  \param output           Iterator at start of output buffer
 *  \param input            Iterator at start of input buffer
 */
template <
    typename Container,
    typename Iter
>
void decodeMessage(Container &output,
    Iter &input)
{
    char *buffer = new char[OUTPUT_INTERVAL];
    for (size_t i = 0; i < OUTPUT_INTERVAL; ++i, ++input) {
        buffer[i] = DECODING[static_cast<int>(*input)];
    }
    // First: 11111111, Second: 00110000
    output.push_back(static_cast<char>((buffer[0] << 2) + ((buffer[1] & 0x30) >> 4)));
    // First: 00001111, Second: 00111100
    output.push_back(static_cast<char>(((buffer[1] & 0x0f) << 4) + ((buffer[2] & 0x3c) >> 2)));
    // First: 00000011, Second: 11111111
    output.push_back(static_cast<char>(((buffer[2] & 0x03) << 6) + buffer[3]));

    delete[] buffer;
}


/** \brief Encode message in start/end range.
 *
 *  Encode bytes within the start/end iterator pair to base64,
 *  adding the bytes to output.
 *
 *  \param container            STL container to store data in
 *  \param output               Iterator at start of output buffer
 *  \param start                Iterator at start of input buffer
 */
template <
    typename Container,
    typename Iter
>
void encode(Container &container,
    Iter start,
    Iter end)
{
    size_t size = encodedSize(end - start);
    container.reserve(size);

    // parse full messages
    while ((start + INPUT_INTERVAL) < end) {
        encodeMessage(container, start);
    }

    // parse last message
    size_t distance = end - start;
    if (distance) {
        // encode message and override empty bytes with '='
        const size_t offset = container.size();
        auto padded = paddedBuffer(start, end, INPUT_INTERVAL);
        auto buffer = padded.cbegin();
        encodeMessage(container, buffer);

        // pad with null characters
        const size_t count = encodedByteCount(distance);
        for (size_t i = count; i < OUTPUT_INTERVAL; ++i) {
            container[offset + i] = '=';
        }
    }
}


/** \brief Decode message in start/end range.
 *
 *  Decoded bytes within the start/end iterator pair from base64,
 *  adding the bytes to output.
 *
 *  \param container            STL container to store data in
 *  \param output               Iterator at start of output buffer
 *  \param start                Iterator at start of input buffer
 */
template <
    typename Container,
    typename Iter
>
void decode(Container &container,
    Iter start,
    Iter end)
{
    size_t size = decodedSize(start, end);
    container.reserve(size);

    // parse full messages
    while ((start + OUTPUT_INTERVAL) < end) {
        decodeMessage(container, start);
    }

    // parse last message
    size_t distance = end - start;
    if (distance) {
        // encode message and override empty bytes with '='
        const size_t offset = container.size();
        auto padded = paddedBuffer(start, end, OUTPUT_INTERVAL, '=');
        auto buffer = padded.cbegin();
        decodeMessage(container, buffer);
    }

    // remove padded bytes
    container.resize(size);
}


}   /* base64 */

// API
// ---

/** \brief Alias for base64::encode.
 */
std::string b64Encode(const std::string &other);

/** \brief String wrapper for base64::decode.
 */
std::string b64Decode(const std::string &other);

#define BASE64_ENCODE b64Encode
#define BASE64_DECODE b64Decode

}   /* lattice */
