#include <stdio.h>
#include <stdint.h>

typedef struct list {
    int value;
    struct list* next;
    uint8_t is_used;
} list_t;

#define LIST_POOL_SIZE 40
static list_t List_pool[LIST_POOL_SIZE];

list_t* get_list_poll_ptr(void) {
    return List_pool;
}

size_t get_list_pool_size(void) {
    return LIST_POOL_SIZE;
}

list_t* list_malloc(void) {
    list_t* result = NULL;
    list_t* p_list = get_list_poll_ptr();
    for (uint32_t i = 0; i < get_list_pool_size(); i++) {
        if (p_list[i].is_used != 1) {
            result = &p_list[i];
            break;
        }
    }
    return result;
}


list_t* list_init(int value) {
    list_t* list = list_malloc();
    if (list != NULL) {
        list->value = value;
        list->next = NULL;
        list->is_used = 1;
    }
    return list;
}

void list_add_start(list_t** head, int value) {
    list_t* list = list_malloc();
    if (list != NULL) {
        list->value = value;
        list->next = *head;
        list->is_used = 1;
        *head = list;
    }
}

list_t* list_find_end(list_t* head) {
    list_t* result = head;
    while (result->next != NULL) {
        result = result->next;
    }
    return result;
}

void list_add_end(list_t* head, int value) {
    list_t* list = list_malloc();
    if (list != NULL) {
        list->value = value;
        list->next = NULL;
        list->is_used = 1;
        list_t* last = list_find_end(head);
        last->next = list;
    }
}

uint32_t list_get_count(list_t* head) {
    list_t* p_list = head;
    uint32_t number = 1;
    while (p_list->next != NULL) {
        number++;
        p_list = p_list->next;
    }
    return number;
}

void list_delete_element(list_t** head, uint32_t num) {
    size_t count = list_get_count(*head);
    list_t* ptr_list = *head;
    list_t* previous_list = NULL;
    if (count >= num) {
        for (uint32_t i = 1; i <= count; i++) {
            if (i == num) {
                ptr_list->is_used = 0;
                ptr_list->value = 0;
                if (i == 1) {
                    *head = (*head)->next;
                }
                else {
                    previous_list->next = ptr_list->next;
                    ptr_list->next = NULL;
                }
                break;
            }
            previous_list = ptr_list;
            ptr_list = ptr_list->next;
        }
    }
}

void list_print_all(list_t* head) {
    list_t* p_list = head;
    for (uint32_t i = 1; i <= list_get_count(head); i++) {
        printf("%u - %u\n", i, p_list->value);
        p_list = p_list->next;
    }
}

void list_sort(list_t* head) {
    uint32_t count = list_get_count(head);
    list_t* p_list = head;
    list_t* active_list = head;
    list_t* offset_list = head;
    uint32_t biggest = 0;
    for (uint32_t i = 0; i < count; i++) {
        biggest = active_list->value;
        p_list = offset_list;
        for (uint32_t j = 0; j < count - i; j++) {
            if (p_list->value > biggest) {
                uint32_t tmp = biggest;
                biggest = p_list->value;
                p_list->value = tmp;
            }
            p_list = p_list->next;
        }
        active_list->value = biggest;
        active_list = active_list->next;
        offset_list = offset_list->next;
    }
}

int main()
{
    list_t* head = list_init(4);
    list_add_end(head, 22);
    list_add_end(head, 76);
    list_add_end(head, 33);
    list_print_all(head);
    list_add_start(&head, 44);
    list_print_all(head);
    list_delete_element(&head, 1);
    list_print_all(head);
    return 0;
}
