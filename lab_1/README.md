# TSU_operating_systems
<h1>
Лабораторная по реализации монитора для потребителя и поставщика 
</h1>
monitor_1.c - пример с буфером из книги Таунбаум
monitor_2.c - лабораторная работа Пожидаева Михаила Сергеевича


<h2>monitor_1.c</h2> 
Предоставленный код реализует пример задачи "производителя-потребителя" с использованием монитора и многопоточности. Эта задача моделирует ситуацию, в которой есть производитель, который помещает данные (записи) в буфер, и потребитель, который извлекает данные из буфера. Задача состоит в том, чтобы правильно синхронизировать их работу, чтобы избежать конфликтов и обеспечить правильное обслуживание буфера.


Заметки:
1.
PTHREAD_MUTEX_INITIALIZER - это специальное значение, которое инициализирует мьютекс в безопасное начальное состояние. Это значит, что мьютекс создается и готов к использованию с адекватными значениями по умолчанию.

Использование PTHREAD_MUTEX_INITIALIZER позволяет избежать неопределенных значений и убедиться, что мьютекс готов к использованию сразу после его создания.

Примеры использования мьютексов включают в себя блокировку доступа к критическим секциям кода, ожидание и уведомление потоков о состоянии общих данных и другие операции синхронизации, чтобы избежать конфликтов и гарантировать правильное выполнение многопоточной программы.

2.
PTHREAD_COND_INITIALIZER - это специальное значение, которое инициализирует условную переменную в безопасное начальное состояние. Это означает, что условная переменная создается и готова к использованию с адекватными значениями по умолчанию.

Использование PTHREAD_COND_INITIALIZER позволяет избежать неопределенных значений и убедиться, что условная переменная готова к использованию сразу после ее создания.

Условные переменные используются в паре с мьютексами для синхронизации между потоками. Обычно один поток ожидает, пока другой поток не изменит какое-то состояние, и затем уведомляет об этом ожидающий поток с помощью условной переменной. Это позволяет одному потоку ожидать определенных условий, прежде чем продолжить выполнение.

3.
pthread_cond_wait - это функция, которая ожидает сигнала (уведомления) на условной переменной, пока другой поток не вызовет pthread_cond_signal или pthread_cond_broadcast для этой условной переменной. Ожидание происходит в критической секции, защищенной мьютексом.

pthread_cond_broadcast - это функция которая используется для отправки сигнала (уведомления) всем потокам, ожидающим на данной условной переменной (pthread_cond_t). Это позволяет активизировать все ожидающие потоки, когда определенное условие или событие наступает. Вызов pthread_cond_broadcast отправляет сигнал всем потокам, которые ожидают на этой условной переменной. Если несколько потоков были заблокированы на этой переменной с помощью pthread_cond_wait, все они будут активизированы.

pthread_cond_broadcast - это функция которая используется для отправки сигнала (уведомления) одному из потоков, ожидающих на данной условной переменной (pthread_cond_t). Это позволяет активизировать только один ожидающий поток, когда определенное условие или событие наступает. Вызов pthread_cond_signal отправляет сигнал одному из потоков, ожидающих на этой условной переменной. Если несколько потоков были заблокированы на этой переменной с помощью pthread_cond_wait, только один из них будет активизирован.

pthread_cond_wait(&not_empty, &mutex);
Функция pthread_cond_wait выполняет следующие действия:

    Разблокирует мьютекс (mutex), который заблокирован текущим потоком. Это позволяет другим потокам получить доступ к общим данным, защищенным этим мьютексом.

    Переводит текущий поток в состояние ожидания на условной переменной (not_empty), пока другой поток не вызовет pthread_cond_signal или pthread_cond_broadcast на той же условной переменной.

    По получении сигнала от другого потока, текущий поток разблокирует мьютекс и продолжит выполнение.

4.
pthread_mutex_lock - это функция в библиотеке POSIX Threads (pthreads), которая используется для захвата (блокировки) мьютекса. Мьютекс используется для синхронизации доступа к разделяемым данным в многопоточной программе.

pthread_mutex_lock(&mutex);

В этом примере:

    &mutex - это указатель на мьютекс, который нужно захватить (заблокировать) текущим потоком.

    Вызов pthread_mutex_lock блокирует мьютекс и предотвращает доступ к коду, охраняемому этим мьютексом, другим потокам. Если мьютекс уже заблокирован другим потоком, текущий поток будет приостановлен (заблокирован) до тех пор, пока мьютекс не освободится другим потоком.

    После успешного выполнения pthread_mutex_lock, мьютекс будет захвачен текущим потоком и предоставит ему доступ к критической секции кода, охраняемой этим мьютексом.

Захват мьютекса обычно используется для создания критических секций, где только один поток может выполнять код в данное время, обеспечивая тем самым безопасность доступа к разделяемым данным и избегая гонок данных (race conditions) в многопоточных приложениях.


<h3>monitor_2.c</h3> 


Заметки:
1. 
Семафоры - это структуры данных, используемые для синхронизации и координации доступа к общим ресурсам в многопоточной программе. Семафоры могут быть двух типов: двоичные и счетчиковые. Двоичный семафор позволяет только одному потоку за раз получать доступ к общим ресурсам, а счетчиковый семафор может позволять нескольким потокам получать доступ к ресурсам в зависимости от значения счетчика. Семафоры могут использоваться для решения проблемы гонки данных (race condition) и дедлока (deadlock) в многопоточной программе.