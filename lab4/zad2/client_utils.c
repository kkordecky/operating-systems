#include <stdio.h>
#include <sys/msg.h>

#include "common.h"

// ===== odbieranie wiadomości =====
void receive_messages(int queue_id) {

    message msg;

    while (1) {

        // odbiór wiadomości
        msgrcv(
            queue_id,
            &msg,
            sizeof(message) - sizeof(long),
            0,
            0
        );

        // wypisanie wiadomości
        printf(
            "Klient %d: %s",
            msg.sender_id,
            msg.text
        );
    }
}