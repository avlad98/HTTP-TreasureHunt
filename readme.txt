VLAD ANDREI-ALEXANDRU
321CB

================================== TEMA 3 PC ==================================
==================================== README ===================================


	Pentru aceasta tema am folosit libraria Parson pentru parsare JSON.

	Am folosit de asemenea codul din laboratorul de http cu mici modificari
in functiile de compunere a mesajelor GET si POST.
	
	Pentru Etapa 1 am trimis o cerere de tip GET catre server si apoi am
urmat pasii indicati la fiecare etapa.
	
	Pentru Etapa 2 am extras cookie-urile primite anterior, le-am pus intr-un
format corespunzator (initial aveam fiecare cookie pe linii diferite, apoi
dupa ce am vazut pe forum, am schimbat formatul in: "
"Cookie: cookie1=val1; cookie2=val2; ...")
	Pentru a alcatui acest string salvez fiecare linie care continie
cookie intr-un vector (cookies_arr), iar la iesirea din parsarea raspunsului
de la server formez String-ul cu cookie-uri.
	De asemenea, aloc o zona de memorie pentru JSON-ul curent si il copiez
cand ajung pe linia care contine "{".
	Apoi extrag din JSON campurile care ma intereseaza si compun mesajul
exact cum se specifica in enuntul raspunsului.
	Pentru a parsa JSON-ul am creat un JSON_Value *json in care se parseaza
linia cu json-ul de la server folosind functia json_parse_string.
	Apoi am stocat valoarea aceasta intr-un JSON_Object *obj folosind
json_value_get_object(json).
	Pentru a extrage campurile de care aveam nevoie am folosit 
json_object_get_string() (valorile campurilor) si json_object_dotget_string()
pentru valorile care erau continute in alte obiecte.
	In toata tema am folosit aceste metode.
	

	Pentru Etapele 3 si 4 am facut aceleiasi pasi, doar ca am extras campuri
diferite acolo unde a fost nevoie.
	Pentru a ma autentifica mereu cu acelasi JWT a fost nevoie sa il salvez
intr-o variabila de header. Am introdus acest header la restul etapelor.

	Pentru Etapa 5 am extras parametrii cu care trebuie sa fac request la
serverul openweather.
	Mai intai, pentru a deschide conexiunea cu acesta, a trebuit sa fac o
cerere de ip la DNS.
	Din raspunsul de la DNS am folosit prima adresa IP si am convertit-o in
ASCII.
	Dupa ce am primit raspunsul de la openweather am salvat pointer-ul catre
linia in care incepe json-ul si l-am trimis serverului temei.
	
	La final am dezalocat memoria folosita pe parcurs.


	Pe masina locala au functionat cererile pana la ultima etapa inclusiv,
obtinand comoara si nota 10 de la server :)



	Output-ul consolei a fost:

	****************** RASPUNS ETAPA 1: ******************
HTTP/1.1 200 OK
Content-Type: application/json; charset=utf-8
Set-Cookie: prajiturica=eyJ1c2VybmFtZSI6ImRNOXJ3dXdRIiwicGFzc3dvcmQiOiJiMFZMR0hVMiIsIl9leHBpcmUiOjE1NTgwMDM0MjA0ODgsIl9tYXhBZ2UiOjM2MDAwMDB9; path=/; expires=Thu, 16 May 2019 10:43:40 GMT; httponly
Set-Cookie: prajiturica.sig=FSFP8wbLjWcJ5edSom64F8PMpLA; path=/; expires=Thu, 16 May 2019 10:43:40 GMT; httponly
X-Response-Time: 1ms
Content-Length: 487
Date: Thu, 16 May 2019 09:43:40 GMT
Connection: keep-alive

{"enunt":"Felicitari! Pentru ca ai facut primul task corect, ai o prajiturica! Iti recomand sa nu o mananci pe drum :) \n  In continuare va trebui sa te loghezi utilizand credentialele oferite in campul data. Te rog sa respecti url-ul, metoda si tipul. \n  Campurile trebuie puse sub forma username=xxx&password=yyy. Ele trebuie extrase, nu hardcodate!","url":"/task2/login","method":"POST","type":"application/x-www-form-urlencoded","data":{"username":"dM9rwuwQ","password":"b0VLGHU2"}}


****************** RASPUNS ETAPA 2: ******************
HTTP/1.1 200 OK
Content-Type: application/json; charset=utf-8
Set-Cookie: prajiturica=eyJ1c2VybmFtZSI6ImRNOXJ3dXdRIiwicGFzc3dvcmQiOiJiMFZMR0hVMiIsInRhc2syIjoiejkxY3o4dmxqdnFndmI5NiIsIl9leHBpcmUiOjE1NTgwMDM0MjU2MDEsIl9tYXhBZ2UiOjM2MDAwMDB9; path=/; expires=Thu, 16 May 2019 10:43:45 GMT; httponly
Set-Cookie: prajiturica.sig=Tt-9y4D_3hO_s4UTh7Akg3w7QEg; path=/; expires=Thu, 16 May 2019 10:43:45 GMT; httponly
X-Response-Time: 8ms
Content-Length: 1463
Date: Thu, 16 May 2019 09:43:45 GMT
Connection: keep-alive

{"enunt":"Felicitari! Ai reusit sa te loghezi cu succes si ai primit un token JWT unic pentru asta si, din nou, o prajiturica!\n  Pentru etapa urmatoare va trebui sa raspunzi corect la urmatoarele 2 intrebari: \"Ce merge dimineata in 4 picioare, la pranz in 2 si seara in 3?\" si \"Ce iti apartine tie, dar oamenii il folosesc mai mult decat il folosesti tu?\"\n  Trimite-mi te rog raspunsurile sub forma de parametri atasati cererii. Fiecare raspuns este un cuvant articulat din limba romana. \n  Vreau, de asemnea, sa imi trimiti si id-ul pe care l-ai primit in queryParams (exemplu: /task3/answer?raspuns1=xxx&raspuns2=yyy&id=zzz).\n  Nu ai voie sa hardcodezi id-ul si valoarea lui, trebuiesc extrase automat!\n\n  Atentie, trebuie sa te autorizezi folosind tokenul JWT. Iti recomand sa citesti putin pe internet. ** Hint: Authorization header **","url":"/task3/answer","method":"GET","data":{"token":"eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6Ino5MWN6OHZsanZxZ3ZiOTYiLCJzY29wZSI6InN0dWRlbnQiLCJpYXQiOjE1NTc5OTk4MjUsImV4cCI6MTU1ODAwMzQyNSwiaXNzIjoiQWxleGFuZHJ1IEhvZ2VhIn0.elovg5y3wILq4tMm8sYNY_6VHnN_BL0MbeWGVnG2QN3PkUtj8EKzOrbKcOa1I6-zWAXtRrKmDs3jj2f414F8oTyDZ0vFg3LR2OGVdFyJpMu-fOVkZ-QjzgJYMQkIwwVP-ItzHrcpgR7zuKxDYgJ06g3mAKBIhxAGQNVM_2bL488Nd3kjmphpGXoYEHpFGOwLTjmf7rdD3kbw-_-Fi33wmugvpNyjZcAwSLcvF3hfj83YYqTwWIXiL4d0QXkM8DR-8mYOoJAs5rQ6Frx_4_znz-a-t2oj2SgelmSGanqm_EJoPVmvPqrWhpGCpDXmoH7Au6pInvZ1NJothzuK0lNVtA","queryParams":{"id":"z91cz8vljvqgvb96"}}}


****************** RASPUNS ETAPA 3: ******************
HTTP/1.1 200 OK
Content-Type: application/json; charset=utf-8
Set-Cookie: prajiturica=eyJ1c2VybmFtZSI6ImRNOXJ3dXdRIiwicGFzc3dvcmQiOiJiMFZMR0hVMiIsInRhc2syIjoiejkxY3o4dmxqdnFndmI5NiIsInRhc2szIjoiY29tcGxldGVkIiwiX2V4cGlyZSI6MTU1ODAwMzQzMDcxNCwiX21heEFnZSI6MzYwMDAwMH0=; path=/; expires=Thu, 16 May 2019 10:43:50 GMT; httponly
Set-Cookie: prajiturica.sig=v_HNRgnZlSpkNT9a2u3XgGxNKo8; path=/; expires=Thu, 16 May 2019 10:43:50 GMT; httponly
X-Response-Time: 2ms
Content-Length: 412
Date: Thu, 16 May 2019 09:43:50 GMT
Connection: keep-alive

{"enunt":"Felicitari! Ai reusit sa raspunzi corect la cele 2 intrebari si ai ajuns la checkpoint!\n  Bineinteles, si de data asta, ai primit prajiturica mult asteptata!\n  Etapa urmatoare consta in obtinerea cheii catre comoara. Este simplu, deci bucura-te de niste puncte obtinute usor!\n  Te rog sa nu uiti ca inca iti trebuie autorizare (nu uita de jwt-ul primit anterior!)","url":"/task4/key","method":"GET"}


****************** RASPUNS ETAPA 4: ******************
HTTP/1.1 200 OK
Content-Type: application/json; charset=utf-8
Set-Cookie: prajiturica=eyJ1c2VybmFtZSI6ImRNOXJ3dXdRIiwicGFzc3dvcmQiOiJiMFZMR0hVMiIsInRhc2syIjoiejkxY3o4dmxqdnFndmI5NiIsInRhc2szIjoiY29tcGxldGVkIiwidGFzazQiOiJyZXRyaWV2ZWQiLCJfZXhwaXJlIjoxNTU4MDAzNDM1ODM0LCJfbWF4QWdlIjozNjAwMDAwfQ==; path=/; expires=Thu, 16 May 2019 10:43:55 GMT; httponly
Set-Cookie: prajiturica.sig=yn6h8-E8F0rbo8ZRUfyRpev09UY; path=/; expires=Thu, 16 May 2019 10:43:55 GMT; httponly
X-Response-Time: 2ms
Content-Length: 729
Date: Thu, 16 May 2019 09:43:55 GMT
Connection: keep-alive

{"enunt":"Felicitari! Mai ai un singur pas pana la comoara. Am nevoie sa imi spui starea vremii in Bucuresti. Pentru asta e nevoie sa faci doua lucruri:\n  1. Trebuie sa dai un request pe url-ul pus in \"data.url\" si sa utilizezi parametrii de cerere pusi in \"data.queryParams\".\n  2. Vreau sa imi trimiti inapoi raspunsul pe noua ruta (url), exact asa cum il primesti. Asta inseamna ca va trebui sa mi-l trimiti in format JSON! \n  Sa nu uiti ca inca trebuie sa fii autorizat. Sper ca nu ai mancat prajiturica!","url":"/task5/treasure","method":"POST","type":"application/json","data":{"url":"api.openweathermap.org/data/2.5/weather","queryParams":{"q":"bucharest","APPID":"80f21fc11662726c0c3c8c5512d89b3a"},"method":"GET"}}


****************** OPENWEATHER: ******************
HTTP/1.1 200 OK
Server: openresty
Date: Thu, 16 May 2019 09:44:00 GMT
Content-Type: application/json; charset=utf-8
Content-Length: 449
Connection: keep-alive
X-Cache-Key: /data/2.5/weather?APPID=80f21fc11662726c0c3c8c5512d89b3a&q=bucharest
Access-Control-Allow-Origin: *
Access-Control-Allow-Credentials: true
Access-Control-Allow-Methods: GET, POST

{"coord":{"lon":26.1,"lat":44.44},"weather":[{"id":803,"main":"Clouds","description":"broken clouds","icon":"04d"}],"base":"stations","main":{"temp":289.02,"pressure":1012,"humidity":87,"temp_min":288.15,"temp_max":292.59},"visibility":10000,"wind":{"speed":5.7,"deg":60},"clouds":{"all":75},"dt":1557999179,"sys":{"type":1,"id":6911,"message":0.0054,"country":"RO","sunrise":1557974867,"sunset":1558028180},"id":683506,"name":"Bucharest","cod":200}


****************** RASPUNS ETAPA 5: ******************
HTTP/1.1 200 OK
Content-Type: text/plain; charset=utf-8
Content-Length: 43
X-Response-Time: 2ms
Date: Thu, 16 May 2019 09:45:05 GMT
Connection: keep-alive

Felicitari, ai obtinut comoara! Ai nota 10!
