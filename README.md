# Internet-Relay-Chat
Communication by channels in group or in private

On utilisera comme client de référence hexchat

## -Guidelines

We Will use the RFC 1459, the full name is Request for comments and it is used to handle the errors in differents informatique material like an IRC chat.

## -PROCESSUS- 


#### Mise en place de la Socket

- [x] Declarations des protocols

- [x] Mettre a l'ecoute

- [x] Lien entre la socket et le setup reseaux

#### Gestion de nouveaux clients

- [ ]  Parsing de reception des messages
    - Utilisation de recv()
    - Traiter le "CAP LS" de la part de Hexchat (ou non)

 - [x] Accepter le nouveau client

 - [ ] Authentification de nickname, password & username
   - Attention au repetitions de check comme des arguments manquants


#### Operateurs de Canaux


- [ ] Channels operateurs a traiter :
  - INVITE
  - JOIN
  - KICK
  - TOPIC

- [ ] Ajouter MODE avec les flags i, t, k, o & l

- [ ] Ajouter la commande PRIVMSG qui gere les messages envoye au channel ou au autre client.

- [ ] Ajouter part pour quitter un channel

#### Numeriques Reply

##     -DOCUMENTATION-

MODERN IRC DOCUMENTATION :
https://modern.ircdocs.horse/#topic-message

RFC 1459 :
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.2

github of IRC by someone :
https://github.com/ngircd/ngircd/tree/master

The self-hosted web IRC client:
https://thelounge.chat/

https://celeo.github.io/2021/06/18/Implementing-an-IRC-server-from-scratch-part-1/

https://www.tala-informatique.fr/wiki/index.php?title=C_socket

https://www.codequoi.com/programmation-reseau-via-socket-en-c/

https://www.youtube.com/watch?v=Y6pFtgRdUts

https://www.youtube.com/watch?v=dEHZb9JsmOU

https://www.softprayog.in/programming/io-multiplexing-select-poll-epoll-in-linux

https://nima101.github.io/io_multiplexing

https://web.maths.unsw.edu.au/~lafaye/CCM/sockets/sockfonc.htm

https://chi.cs.uchicago.edu/chirc/irc_examples.html

https://chi.cs.uchicago.edu/chirc/irc.html

https://www.tala-informatique.fr/wiki/index.php?title=C_socket

https://alien.net.au/irc/irc2numerics.html

https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9

https://bruno.univ-tln.fr/enseignement/reseaux/tp/tp3

https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/

https://www.tala-informatique.fr/wiki/index.php?title=C_socket

http://manpagesfr.free.fr/man/man2/socket.2.html

http://manpagesfr.free.fr/man/man2/getsockopt.2.html

http://manpagesfr.free.fr/man/man2/bind.2.html

http://manpagesfr.free.fr/man/man2/listen.2.html

http://manpagesfr.free.fr/man/man2/poll.2.html

http://manpagesfr.free.fr/man/man2/select.2.html

http://manpagesfr.free.fr/man/man2/accept.2.html

http://manpagesfr.free.fr/man/man2/connect.2.html

http://manpagesfr.free.fr/man/man2/send.2.html

http://manpagesfr.free.fr/man/man2/recv.2.html

https://dd.ircdocs.horse/refs/numerics/001

https://modern.ircdocs.horse/#privmsg-message

https://modern.ircdocs.horse/#rplwelcome-001

https://en.wikipedia.org/wiki/ANSI_escape_code

https://stackoverflow.com/questions/10606101/automatically-add-all-submodules-to-a-repo

https://script.quakenet.org/wiki/Category:Raws

https://gnome.pages.gitlab.gnome.org/gnome-browser-integration/extension-12.html
