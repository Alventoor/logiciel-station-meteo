#ifndef FATALEXCEPTION_H
#define FATALEXCEPTION_H

#include <exception>

#include <QByteArray>
#include <QString>

/**
 * @brief Classe FatalException
 *
 * Cette classe permet de lever des exceptions fatales au programme. De ce fait
 * ces exceptions doivent uniquement être traitées dans la fonction principale,
 * leur rôle étant de permettre de quitter le programme correctement.
 */
class FatalException : public std::exception {
    public:
        /**
         * @brief Construit une nouvelle FatalException.
         * @param msg message d'information sur l'origine de l'exception
         */
        FatalException(const QString &msg);

        /**
         * @brief retourne un message d'information sur l'origine de l'exception.
         * @return le message d'information sur l'origine de l'exception
         */
        const char *what() const noexcept override;

    private:
        /**
         * @brief message d'information sur l'origine de l'exception.
         */
        QByteArray msg;
};

#endif // FATALEXCEPTION_H
