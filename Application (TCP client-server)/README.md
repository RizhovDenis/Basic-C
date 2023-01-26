# Instructions
Hey there! This app can create user's accounts. When users can chatting each other. Just use server.c and client.c scripts.

## Without TCP

* For creating account see next string:
~~~
./create-user --login=example --pasword=example --name=example --lastname=example --age=example
~~~
* For open account write your data:
~~~
./read-user --login=example --password=example
~~~
* Check senders and count messages:
~~~
./read-mailbox --login=example --password=example
~~~
* Read message:
~~~
./read-mail --login=example --password=example --message-id=3
~~~

* Send message:
~~~
./send-message --from=example --password=example --destination=example --message='What are you waiting for?'
~~~

## TCP commands
* Launch your server:
~~~
./server start --host=127.0.0.1 --port=9090
~~~
* Enter users data for chatting:
~~~
./client --host=127.0.0.1 --port=9090 --login=example --password=example
~~~
