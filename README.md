## Лаборатоная работа №3 по Операционным системам

### Вариант: procfs: ss

Для сборки достаточно просто ввести команду `make`. После этого будет собраны 2 файла. Первый **my_ss** - пользовательская программа, которая принимает 0 или 1 аргумент.

Возможные аргументы:
- _без аргументов_ - выводятся информация о всех подключениях tcp и udp.
- `tcp` - выводится информация только о tcp соединениях
- `udp` - выводится информация только о udp соединениях
- _фильтр tcp подключений_ - выводятся только tcp подключения с указанным в аргументе состояниями:
    - ESTABLISHED
    - SYN_SENT
    - SYN_RECV
    - FIN_WAIT1
    - FIN_WAIT2
    - TIME_WAIT
    - CLOSE
    - CLOSE_WAIT
    - LAST_ACK
    - LISTEN
    - CLOSING
    - NEW_SYN_RECV

Второй файл - **my_ss_module.ko** - модуль ядра, принимающий от пользовательской программы её аргументы. Для загрузки модуля ядра введите команду
```bash
$ sudo insmod my_ss_module.ko
```
В списке встроенных модулей ядра должен появиться данный модуль
```bash
$ sudo lsmod | grep my_ss_module
```
Вывод:
```
my_ss_module           12288  0
```
Также в директории `/proc` должен создасться файл `my_ss_module`.

Первым этапом нужно передать ядру аргументы строки (или без аргументов).
```bash
$ ./my_ss udp
```
Вторым и последним этапом достаточно всего лишь считать содержимое файла `/proc/my_ss_module`.
```bash
$ cat /proc/my_ss_module
```
В данном случае нам выведятся все udp соединения в системе.
```
Recv-Q   Send-Q   Local Address:Port   Remote Address:Port 
0        0        0.0.0.0:47896        0.0.0.0:0     
0        0        0.0.0.0:39787        0.0.0.0:0     
0        0        0.0.0.0:5353         0.0.0.0:0     
0        0        0.0.0.0:5353         0.0.0.0:0     
0        0        127.0.0.53:53        0.0.0.0:0     
0        0        10.0.2.15:68         10.0.2.2:67
```

### Примеры

![1](https://github.com/IoannTar2004/os-lab3/imgs/1.jpg)

![2](https://github.com/IoannTar2004/os-lab3/imgs/2.jpg)