#include <errorsystem.h>

QString get_error_message(int error_num) {
    QString error_message;
    switch (error_num) {
    case NO_ERROR:
        error_message = "File is saved";
        break;
    case FILE_EXISTS_ERROR:
        error_message = "You already have a level with that name. Try another.";
        break;
    case WRONG_CHARECTER_ERROR:
        error_message = "It seem that there is not cool charecters in your level name.";
        break;
    case NO_NAME_ERROR:
        error_message = "You must give a name to your level.";
        break;
    default:
        break;
    }

    return error_message;
}
