# Internet-Relay-Chat
Communication by channels in group or in private


## -Guidelines
We Will use hexchat as a client reference.
We Will use the base of the RFC 1459 and add the required of the modern.
The full name is Request For Comments and it is used to handle the errors in differents informatique material like an IRC chat.

## -PROCESSUS- 


#### Mise en place de la Socket

- [x] Declarations des protocols

- [x] Mettre a l'ecoute

- [x] Lien entre la socket et le setup reseaux

#### Gestion de nouveaux clients

- [x]  Parsing de reception des messages
    - Utilisation de recv()
    - Traiter le "CAP LS" de la part de Hexchat (ou non)

 - [x] Accepter le nouveau client

 - [x] Authentification de nickname, password & username
   - Attention au repetitions de check comme des arguments manquants


#### Operateurs de Canaux


- [x] Channels operateurs a traiter :
  - INVITE
  - JOIN
  - KICK
  - TOPIC

- [x] Ajouter MODE avec les flags i, t, k, o & l

- [x] Ajouter la commande PRIVMSG qui gere les messages envoye au channel ou au autre client.

- [x] Ajouter part pour quitter un channel

##     -DOCUMENTATION-

Modern Irc Documentation :

https://modern.ircdocs.horse/#topic-message


Numerique Reply & Error Message :

https://script.quakenet.org/wiki/Categories


Server Fuction & Explaination :

https://www.tala-informatique.fr/wiki/index.php?title=C_socket


Rfc 1459 :

https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.2

	// Fast checkin
	if (buff == "ff\r") {
		Pass("pass", fd_cli);
		Nick("defni", fd_cli);
		User("defus 0 * :noreal", fd_cli);
		return (3);
	}
	if (buff == "fff\r") {
		Pass("pass", fd_cli);
		Nick("fn", fd_cli);
		User("fu 0 * :norh", fd_cli);
		return (3);
	}
	//End Fast Check