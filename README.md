
Maria-Alexandra Barbu, 325CD
Tema 3- Protocoale de comunicatii- Client Web. Comunicatie cu REST API
-------------------------------------------------------------------------------

	Am folosit "telnet" ca sa gasesc forma corecta a fiecarui tip de cerere.
Fisierele "buffer.c", "helpers.c", "helpers.h", le-am luat din Laboratorul 10.
Pentru fiecare tip de cerere am construit o functie. Nu am folosit o anumita 
biblioteca de parsare JSON, ci am folosit "sprintf" pentru a compune mesajul.
Am utilizat functia "compute_message" din laborator. Pentru cererea de
"add_book" am citit cu "fgets" de la tastatura toate campurile necesare, dar
apoi a trebuit sa elimin newline-ul de la final. In functia main am facut
scrierea si citirea de la tastatura pentru client, am apelat functia necesara
pentru formarea unui anumit tip de cerere, apoi am trimis mesajul catre server
folosind functia "send_to_server" si am primit raspunsul folosind
"receive_from_server". Pentru a accesa anumite parti din raspunsul primit de la
server (de exemplu codul, pentru a printa mesajele necesare de eroare), am
folosit functia "strtok". Programul afiseaza cate un mesaj si in caz de succes,
dar si in cazuri de eroare. 


































