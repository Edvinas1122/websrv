#ifndef RETURN_CODES_H
# define RETURN_CODES_H

# define SUCESS 0
# define FAILED_SOCKET -1
# define LIST_END NULL
# define LAST_BUFFER true
# define NOT_LAST_BUFFER false

#ifdef CPP98
# define EXCEPTION throw(std::exception)
// # define EXCEPITON(x) throw(x)
#else
# define EXCEPTION noexcept(false)
// # define EXCEPITON(x) noexcept(false)
#endif
/*  */

// https://www.rfc-editor.org/rfc/rfc2616#page-53 <- methods

/* HTTP METHODS */
# define GET_METHOD 1
# define HEAD_METHOD 2

# define POST_METHOD 3
# define PUT_METHOD 4

# define DELETE_METHOD 5
# define TRACE_METHOD 6

# define CONNECT_METHOD 7
# define OPTIONS_METHOD 8

/* HTTP version */
# define VERSION_1_0 1
# define VERSION_1_1 2

/*error codes*/
#define	OK 200

#define BAD 400

#endif