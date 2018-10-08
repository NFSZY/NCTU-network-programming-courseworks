/*
 *  網路程式設計 hw2 - Socket Client
 *
 *  0416060 張楹翔
 *  2018/10/08
 *
 *  Compile: g++ main.cpp -ljson-c
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <map>
#include <string>
#include <json-c/json.h>
using namespace std;

// ========== Function Declarations ==========
void execute( const char* command );
int sendSocket( const char* msg, char* res, size_t res_buf_size);
int parseJson( const char* label, const char* js_str, int* value );
int parseJson( const char* label, const char* js_str, char* value );
int printJsonList( const char* label, const char* js_str );
int printJsonPost( const char* js_str );

// Commands
void doRegister();
void doLogin();
void doDelete();
void doLogout();
void doInvite();
void doListInvite();
void doAcceptInvite();
void doListFriend();
void doPost();
void doReceivePost();
void cleanup();


// ========== Global variables ==========
char* ip;
int port;
map<string, string> tokens;


// ========== Main Function ==========
int main( int argc, char* argv[] ) {

    if ( argc != 3 ) {
        cout << argc << endl;
        cout << "You should specify <IP> and <PORT> in command line arguments" << endl;
        return 0;
    }

    // Parse <IP> and <PORT> from command line arguments
    ip = argv[1];
    port = atoi(argv[2]);

    char command[20];
    scanf("%s", command);
    while ( strcmp(command, "exit") != 0 ) {
        execute(command);
        scanf("%s", command);
    }

    return 0;
}


// ========== Function Implementations ==========
void execute( const char* command ) {
    if ( strcmp(command, "register") == 0 ) {
        doRegister();
    } else if ( strcmp(command, "login") == 0 ) {
        doLogin();
    } else if ( strcmp(command, "delete") == 0 ) {
        doDelete();
    } else if ( strcmp(command, "logout") == 0 ) {
        doLogout();
    } else if ( strcmp(command, "invite") == 0 ) {
        doInvite();
    } else if ( strcmp(command, "list-invite") == 0 ) {
        doListInvite();
    } else if ( strcmp(command, "accept-invite") == 0 ) {
        doAcceptInvite();
    } else if ( strcmp(command, "list-friend") == 0 ) {
        doListFriend();
    } else if ( strcmp(command, "post") == 0 ) {
        doPost();
    } else if ( strcmp(command, "receive-post") == 0 ) {
        doReceivePost();
    }
}

int sendSocket( const char* msg, char* res, size_t res_buf_size ) {

    // Create socket object
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if ( sock == -1 ) {
        cout << "Fail to create a socket" << endl;
        return 0;
    }

    // Init server address info
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // ipv4
    serv_addr.sin_addr.s_addr = inet_addr(ip); // ip
    serv_addr.sin_port = htons(port); // port
    //cout << "try to connect" << endl;
    // Connect to server
    if ( connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1 ) {
        cout << "Fail to create connection to server" << endl;
        return 0;
    }
    //cout << "connect done" << endl;
    // Send message
    if ( send(sock, msg, strlen(msg), 0) == -1 ) {
        cout << "Fail to send message to server" << endl;
        return 0;
    }

    // Receive response
    if ( recv(sock, res, res_buf_size, 0) == -1 ) {
        cout << "Fail to receive response from server" << endl;
        return 0;
    }

    close(sock);

    return 1;
}

int parseJson( const char* label, const char* js_str, int* value ) {
    struct json_object *root, *target;

    root = json_tokener_parse(js_str);
    /*if ( is_error(j_root) ) {
        cout << "Json parse failed" << endl;
        json_object_put(root); // free resource
        return -1;
    }*/

    target = json_object_object_get(root, label);
    if ( !target ) {
        cout << "Fail to parse " << label << " from json" << endl;
        json_object_put(root); // free resource
        return 0;
    }

    *value = json_object_get_int(target);

    json_object_put(root); // free resource
    return 1;
}

int parseJson( const char* label, const char* js_str, char* value ) {
    struct json_object *root, *target;

    root = json_tokener_parse(js_str);
    /*if ( is_error(j_root) ) {
        cout << "Json parse failed" << endl;
        json_object_put(root); // free resource
        return -1;
    }*/

    target = json_object_object_get(root, label);
    if ( !target ) {
        cout << "Fail to parse " << label << " from json" << endl;
        json_object_put(root); // free resource
        return 0;
    }

    strcpy(value, json_object_get_string(target));

    json_object_put(root); // free resource
    return 1;
}


int printJsonList( const char* label, const char* js_str ) {
    struct json_object *root, *target;

    root = json_tokener_parse(js_str);
    if ( root == NULL ) {
        cout << "Json parse failed" << endl;
        return 0;
    }

    target = json_object_object_get(root, label);
    if ( !target ) {
        cout << "Fail to parse " << label << " from json" << endl;
        json_object_put(root); // free resource
        return 0;
    }

    int len = json_object_array_length(target);
    for ( int i = 0; i < len; ++i ) {
        cout << json_object_get_string(json_object_array_get_idx(target, i)) << endl;
    }

    json_object_put(root); // free resource
    return 1;
}

int printJsonPost( const char* js_str ) {

    struct json_object *root, *posts;

    root = json_tokener_parse(js_str);
    if ( root == NULL ) {
     cout << "Json parse failed" << endl;
     return 0;
    }

    posts = json_object_object_get(root, "post");
    if ( !posts ) {
        cout << "Fail to parse posts from json" << endl;
        json_object_put(root); // free resource
        return 0;
    }

    int len = json_object_array_length(posts);
    for ( int i = 0; i < len; ++i ) {
        struct json_object *post_obj, *id_obj, *msg_obj;
        post_obj = json_object_array_get_idx(posts, i);
        id_obj = json_object_object_get(post_obj, "id");
        msg_obj = json_object_object_get(post_obj, "message");
        cout << json_object_get_string(id_obj) << ": " << json_object_get_string(msg_obj) << endl;
    }

    // free resources
    json_object_put(root);

    return 1;
}


// Commands
void doRegister() {
    char id[30];
    char password[30];
    scanf("%s %s", id, password);

    char req_msg[70] = "register ";
    strcat(req_msg, id);
    strcat(req_msg, " ");
    strcat(req_msg, password);
    char res[300];
    if ( sendSocket(req_msg, res, 300) ) {
        char res_msg[50];
        if ( parseJson("message", res, res_msg) ) {
            cout << res_msg << endl;
        }
    }
}


void doLogin() {
    char id[30];
    char password[30];
    scanf("%s %s", id, password);

    char req_msg[70] = "login ";
    strcat(req_msg, id);
    strcat(req_msg, " ");
    strcat(req_msg, password);

    char res[300];
    if ( sendSocket(req_msg, res, 300) ) {
        int status;
        if ( parseJson("status", res, &status) ) {
            if ( status == 0 ) {
                // Success
                char token[70];
                if ( parseJson("token", res, token) ) {
                    tokens.insert(pair<string, string>((string)id, (string)token));
                }
            }
        }
        char res_msg[50];
        if ( parseJson("message", res, res_msg) ) {
            cout << res_msg << endl;
        }
    }
}

void doDelete() {
    char user[30];
    scanf("%s", user);

    char req_msg[100] = "delete ";
    strcat(req_msg, tokens.find((string)user)->second.c_str());

    char res[300];
    if ( sendSocket(req_msg, res, 300) ) {

        int status;
        if ( parseJson("status", res, &status) ) {
            if ( status == 0 ) {
                // Success
                tokens.erase((string)user);
            }
        }

        char res_msg[50];
        if ( parseJson("message", res, res_msg) ) {
            cout << res_msg << endl;
        }
    }
}

void doLogout() {
    char user[30];
    scanf("%s", user);

    char req_msg[100] = "logout ";
    strcat(req_msg, tokens.find((string)user)->second.c_str());

    char res[300];
    if ( sendSocket(req_msg, res, 300) ) {

        int status;
        if ( parseJson("status", res, &status) ) {
            if ( status == 0 ) {
                // Success
                tokens.erase((string)user);
            }
        }

        char res_msg[50];
        if ( parseJson("message", res, res_msg) ) {
            cout << res_msg << endl;
        }
    }
}

void doInvite() {
    char user[30];
    char id[30];
    scanf("%s %s", user, id);

    char req_msg[130] = "invite ";
    strcat(req_msg, tokens.find((string)user)->second.c_str());
    strcat(req_msg, " ");
    strcat(req_msg, id);

    char res[300];
    if ( sendSocket(req_msg, res, 300) ) {
        char res_msg[50];
        if ( parseJson("message", res, res_msg) ) {
            cout << res_msg << endl;
        }
    }
}

void doListInvite() {
    char user[30];
    scanf("%s", user);

    char req_msg[100] = "list-invite ";
    strcat(req_msg, tokens.find((string)user)->second.c_str());

    char res[300];
    if ( sendSocket(req_msg, res, 300) ) {

        int status;
        if ( parseJson("status", res, &status) ) {
            if ( status == 0 ) {
                // Success
                printJsonList("invite", res);
            } else {
                // Fail
                char res_msg[50];
                if ( parseJson("message", res, res_msg) ) {
                    cout << res_msg << endl;
                }
            }
        }
    }
}

void doAcceptInvite() {
    char user[30];
    char id[30];
    scanf("%s %s", user, id);

    char req_msg[130] = "accept-invite ";
    strcat(req_msg, tokens.find((string)user)->second.c_str());
    strcat(req_msg, " ");
    strcat(req_msg, id);

    char res[300];
    if ( sendSocket(req_msg, res, 300) ) {
        char res_msg[50];
        if ( parseJson("message", res, res_msg) ) {
            cout << res_msg << endl;
        }
    }
}

void doListFriend() {
    char user[30];
    scanf("%s", user);

    char req_msg[100] = "list-friend ";
    strcat(req_msg, tokens.find((string)user)->second.c_str());

    char res[300];
    if ( sendSocket(req_msg, res, 300) ) {
        int status;
        if ( parseJson("status", res, &status) ) {
            if ( status == 0 ) {
                // Success
                printJsonList("friend", res);
            } else {
                // Fail
                char res_msg[50];
                if ( parseJson("message", res, res_msg) ) {
                    cout << res_msg << endl;
                }
            }
        }
    }
}

void doPost() {
    char user[30];
    char post[100];
    scanf("%s ", user);
    cin.getline(post, 100);

    char req_msg[230] = "post ";
    strcat(req_msg, tokens.find((string)user)->second.c_str());
    strcat(req_msg, " ");
    strcat(req_msg, post);

    char res[300];
    if ( sendSocket(req_msg, res, 300) ) {
        char res_msg[50];
        if ( parseJson("message", res, res_msg) ) {
            cout << res_msg << endl;
        }
    }
}

void doReceivePost() {
    char user[30];
    scanf("%s", user);

    char req_msg[100] = "receive-post ";
    strcat(req_msg, tokens.find((string)user)->second.c_str());

    char res[500];
    if ( sendSocket(req_msg, res, 500) ) {

        int status;
        if ( parseJson("status", res, &status) ) {
            if ( status == 0 ) {
                // Success
                printJsonPost(res);
            } else {
                // Fail
                char res_msg[50];
                if ( parseJson("message", res, res_msg) ) {
                    cout << res_msg << endl;
                }
            }
        }
    }
}
