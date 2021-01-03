#include "fatalexception.h"

FatalException::FatalException(const QString &msg) {
    this->msg = msg.toLocal8Bit();
}

const char *FatalException::what() const noexcept {
    return msg.constData();
}
