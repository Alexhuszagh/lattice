//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Type detection utiltiies.
 */

#pragma once

#include <type_traits>


namespace lattice
{
// MEMBERS
// -------


/** \brief Check if class has generic method.
 *
 *  Uses Substitution Failure Is Not An Error (SFINAE) to determine
 *  if the class has a member function with the given name.
 */
#define HAS_MEMBER_FUNCTION(func, name)                                      \
    template<typename T>                                                     \
    class name                                                               \
    {                                                                        \
    protected:                                                               \
        template <typename C> static char &test(decltype(&C::func));         \
        template <typename C> static long &test(...);                        \
                                                                             \
    public:                                                                  \
        enum {                                                               \
            value = sizeof(test<T>(0)) == sizeof(char)                       \
        };                                                                   \
    };                                                                       \


// FUNCTIONS
// ---------


/** \brief Has the setReuseAddress method, to prevent address blocking.
 */
HAS_MEMBER_FUNCTION(setReuseAddress, HasSetReuseAddress);

/** \brief Has the setTimeout method, to timeout requests.
 */
HAS_MEMBER_FUNCTION(setTimeout, HasSetTimeout);

/** \brief Has the setCertificateFile method, to set a certificate file.
 */
HAS_MEMBER_FUNCTION(setCertificateFile, HasSetCertificateFile);

/** \brief Has the setRevocationLists method, to manually revoke certificates.
 */
HAS_MEMBER_FUNCTION(setRevocationLists, HasSetRevocationLists);

/** \brief Has the setSslProtocol method, to specify the TLS implementation.
 */
HAS_MEMBER_FUNCTION(setSslProtocol, HasSetSslProtocol);

/** \brief Has the setVerifyPeer method, to toggle peer validation.
 */
HAS_MEMBER_FUNCTION(setVerifyPeer, HasSetVerifyPeer);

}   /* lattice */
