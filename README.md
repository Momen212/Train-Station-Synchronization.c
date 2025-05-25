 # Synchronisation mit Mutex

Dieses Programm simuliert einen Bahnhof mit begrenzten Gleisen (z.B. 5). Jeder Zug ist ein eigener Thread.
Ein pthread_mutex_t schützt den Zugriff auf eine gemeinsame Variable occupiedTracks, die zählt, wie viele Gleise gerade belegt sind.
Züge dürfen nur einfahren, wenn mindestens ein Gleis frei ist. Andernfalls warten sie kurz und versuchen es erneut.

Ziel: Synchronisation über manuelles Zählen + Mutex.

 # Synchronisation mit POSIX-Semaphor

In dieser Version wird statt eines Mutex ein POSIX-Semaphor (sem_t) verwendet.
Der Semaphor wird mit der Anzahl der verfügbaren Gleise (z. B. 5) initialisiert und übernimmt die Kontrolle über den Zugriff automatisch.
Züge rufen sem_wait() auf, um ein Gleis zu reservieren, und sem_post() beim Verlassen, um es freizugeben.

Ziel: Synchronisation eleganter und direkter über Ressourcen-Zähler (Semaphore).

Beide Programme stellen sicher, dass nie mehr als 5 Züge gleichzeitig im Bahnhof sind – aber mit unterschiedlichen Techniken.
