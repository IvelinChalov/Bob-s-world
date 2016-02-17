#ifndef ERRORSYSTEM
#define ERRORSYSTEM
#include <QString>

enum {
    NO_ERROR = 0,
    FILE_EXISTS_ERROR = 1,
    WRONG_CHARECTER_ERROR = 2,
    NO_NAME_ERROR = 3
}ErrorMessages;

QString get_error_message(int error_num);
#endif // ERRORSYSTEM

