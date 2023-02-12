#include <pthread.h>
#include <iostream>
#include <string>

using namespace std;

struct ThreadData {
    char character;
    string *message;
    int *frequency;
};

void *count_frequency(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    char character = data->character;
    string message = *data->message;
    int count = 0;
    for (char c : message) {
        if (c == character) {
            count++;
        }
    }
    *data->frequency = count;
    pthread_exit(NULL);
}

int main() {
    int n;
    cin >> n;

    string characters="C 3";
    // cin >> characters;

    string message = "COSC 3360 FALL 2022 - RINCON";
    // cin.ignore();
    // getline(cin, message);

    pthread_t threads[n];
    int frequencies[n];
    ThreadData data[n];

    for (int i = 0; i < n; i++) {
        data[i].character = characters[i];
        data[i].message = &message;
        data[i].frequency = &frequencies[i];
        int rc = pthread_create(&threads[i], NULL, count_frequency, (void *)&data[i]);
        if (rc) {
            cout << "Error: unable to create thread, " << rc << endl;
            exit(-1);
        }
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    cout << "Frequencies:" << endl;
    for (int i = 0; i < n; i++) {
        cout << characters[i] << ": " << frequencies[i] << endl;
    }

    pthread_exit(NULL);
    return 0;
}
