#include "UserFunctions.h"
#include "PreFunctions.h"

FILE *users_fptr;
char *warning;
user *users;

int LoginUser()
{
    CommandHeading("Log In");

    char *loginuser = (char *) malloc (32 * sizeof(char));

    if((users_fptr = fopen("Docs/Users.txt", "a+")) == NULL)
    {
        strcpy(warning, "COULD NOT OPEN FILE");
        Warning(0);
        return 0;
    }

    fscanf(users_fptr, "%d", &numberofusers);
    if(numberofusers == 0)
    {
        strcpy(warning, "FILE IS EMPTY");
        Warning(0);
        CreateAdmin();
    }
    else
    {
        users = (user *) realloc (users, numberofusers * sizeof(user));//// size of user ?
        for(int i = 0; i < numberofusers; ++i)
            fscanf(users_fptr, "%s %s %s %lld %lld %d %d", users[i].name, users[i].username, users[i].password, &users[i].accesstime, &users[i].starttime, &users[i].accesslevel, &users[i].active);
        fclose(users_fptr);    
    }

    loginuser = GetInfo("USERNAME", 1);///// what doing?

    int i;
    for(i = 0; i < numberofusers; ++i)
    {
        if(!strcmp(users[i].username, loginuser))
        {
            if(CheckPassword(users[i].password) == 1)
            {
                strcpy(currentuser.name, users[i].name);
                strcpy(currentuser.username, users[i].username);
                strcpy(currentuser.password, users[i].password);
                currentuser.accesslevel = users[i].accesslevel;
                currentuser.accesstime = users[i].accesstime;
                currentuser.active = 1;
                currentuser.starttime = time(NULL);
                SaveUsersLog("enter");
                ClearScreen();
                return 1;
            }
            break;
        }
    }

    if(i == numberofusers)
    {
        strcpy(warning, "USER NOT FOUND");
        Warning(0);
    }
    return 0;
}

int SwitchUser()
{
    CommandHeading("Switch User");

    char lastuser[32] = {};
    strcpy(lastuser, currentuser.username);

    char *loginuser = (char *) malloc (32 * sizeof(char));

    loginuser = GetInfo("USERNAME", 1);

    for(int i = 0; i < numberofusers; ++i)
    {
        if(!strcmp(users[i].username, loginuser))
        {
            if((currentuser.accesslevel != 1 && CheckPassword(users[i].password) == 1) || (currentuser.accesslevel == 1))
            {
                strcpy(currentuser.name, users[i].name);
                strcpy(currentuser.username, users[i].username);
                strcpy(currentuser.password, users[i].password);
                currentuser.accesslevel = users[i].accesslevel;
                currentuser.accesstime = users[i].accesstime;
                currentuser.active = 1;
                currentuser.starttime = time(NULL);
                SaveUsersLog("exit");///////ffffff
                SaveUsersLog("enter");//////ffffff
                if(strcmp(lastuser, loginuser) != 0)
                {
                    strcpy(warning, lastuser);
                    strcat(warning, " LOGGED OUT");
                    Warning(1);
                    strcpy(warning, "USER LOGGED IN SUCCESSFULLY");
                    Warning(1);
                }
                else
                {
                    strcpy(warning, lastuser);
                    strcpy(warning, "ENTERED AGAIN");
                    Warning(1);
                }
                return 1;
            }
            break;
        }
    }

    return 0;
}

int CreateUser()
{
    CommandHeading("Create User");

    users = (user *) realloc (users, (numberofusers + 1) * sizeof(user));

    strcpy(users[numberofusers].name, GetInfo("NAME", 1));
    strcpy(users[numberofusers].username, GetInfo("USERNAME", 1));

    int i = 0;
    while(i < numberofusers)
    {
        if(strcmp(users[numberofusers].username, users[i].username) == 0)
        {
            strcpy(warning, "USED USERNAME");
            Warning(0);
            strcpy(users[numberofusers].username, GetInfo("USERNAME", 1));
            i = 0;
        }
        else
            i++;
    }

    strcpy(users[numberofusers].password, GetInfo("PASSWORD", 1));
    users[numberofusers].accesstime = 1000;
    users[numberofusers].active = 0;
    users[numberofusers].accesslevel = 2;
    users[numberofusers].starttime = time(NULL);
    numberofusers++;

    SaveUsers();
    return 1;
}

int ListUsers()
{
    CommandHeading("List Users");

    if( (users_fptr = fopen("Docs/Users.txt","r")) == NULL)
    {
        strcpy(warning, "COULD NOT OPEN FILE");
        Warning(0);
        return 0;
    }

    printf("    \033[1m%-20s%-20s%-20s%s\n\033[0m", "NAME", "USERNAME", "ACCESS TIME", "ACCESS LEVEL");
    for(int i = 0; i < numberofusers; ++i)
        printf("    %-20s%-20s%-20lld%d\n", users[i].name, users[i].username, users[i].accesstime, users[i].accesslevel);

    printf("    Total: %d\n", numberofusers);

    fclose(users_fptr);
    return 1; 
}

int LogoutUser()
{
    strcpy(warning, "USER ");
    strcat(warning, currentuser.name);
    strcat(warning, " LOGGED OUT");
    Warning(1);
    strcpy(currentuser.name, "");
    strcpy(currentuser.password, "");
    currentuser.accesslevel = 0;
    currentuser.accesstime = 0;
    currentuser.active = 0;
    SaveUsersLog("exit");

    return 1;
}

void SaveUsers() //////ffffffff
{
    if((users_fptr = fopen("Docs/Users.txt","w+")) == NULL)
    {
        strcpy(warning, "COULD NOT OPEN FILE");
        Warning(0);
    }

    fprintf(users_fptr, "%d\n", numberofusers);
    for(int i = 0; i < numberofusers; ++i)
        fprintf(users_fptr, "%s %s %s %lld %lld %d %d\n", users[i].name, users[i].username, users[i].password, users[i].accesstime, users[i].starttime, users[i].accesslevel, users[i].active);

    fclose(users_fptr);
}

void CreateAdmin()
{
    users_fptr = fopen("Docs/Users.txt", "w+");
    strcpy(users->name, "admin");
    strcpy(users->username , "admin");
    strcpy(users->password , "admin");
    users->accesstime = 10000000;
    users->starttime = time(NULL);
    users->accesslevel = 1;
    users->active = 1;

    numberofusers = 1;
    strcpy(warning, "ADMIN HAS BEEN CREATED");
    Warning(1);

    rewind(users_fptr);////////ffffff
    fprintf(users_fptr, "1\n");
    fprintf(users_fptr, "%s %s %s %lld %lld %d %d\n", users[0].name, users[0].username, users[0].password, users[0].accesstime, users[0].starttime, users[0].accesslevel, users[0].active);

    fclose(users_fptr);
}

int DeleteUser()
{
    char* temp;
    temp = GetInfo("USERNAME", 1);
    for(int i = 0; i < numberofusers; ++i)
        if(!strcmp(users[i].username, temp))
        {
            if(users[i].accesslevel == 1)
            {
                strcpy(warning, "ADMINS CANNOT BE DELETED");
                Warning(0);
                return 0;
            }
            else
            {
                strcpy(warning, "YOU ARE ABOUT TO DELETE A USER");
                Warning(0);
                char *agree;
                agree = GetInfo("AGREE(Y/N)", 1);
                if((*agree == 'Y' || *agree == 'y') && (strlen(agree) == 1))
                {
                    strcpy(users[i].name, users[numberofusers - 1].name);
                    strcpy(users[i].username, users[numberofusers - 1].username);
                    strcpy(users[i].password, users[numberofusers - 1].password);
                    users[i].accesslevel = users[numberofusers - 1].accesslevel;
                    users[i].starttime = users[numberofusers - 1].starttime;
                    users[i].active = users[numberofusers - 1].active;
                    users[i].accesstime = users[numberofusers - 1].accesstime;
                    strcpy(users[numberofusers - 1].name, "");
                    strcpy(users[numberofusers - 1].username, "");
                    strcpy(users[numberofusers - 1].password, "");
                    users[numberofusers - 1].accesslevel = 0;
                    users[numberofusers - 1].starttime = 0;
                    users[numberofusers - 1].active = 0;
                    users[numberofusers - 1].accesstime = 0;
                    numberofusers--;
                    users = (user *) realloc (users, numberofusers * sizeof(user));
                    SaveUsers();
                    strcpy(warning, "USER DELETED SUCCESSFULLY");
                    Warning(1);
                    return 1;
                }
                else
                {
                    strcpy(warning, "DELETEIN CANCELED");
                    Warning(0);
                    return 0;
                }
    
            }
        }

    strcpy(warning, "USER NOT FOUND");
    Warning(0);
    return 0;
}

void ChangePassword(char* username)
{
    CommandHeading("Change Password");

    int i = 0;
    for(i = 0; i < numberofusers; ++i)
        if(!strcmp(users[i].username, username))
        {
            strcpy(users[i].password, GetInfo("NEW PASSWORD", 1));
            SaveUsers();
            strcpy(warning, "PASSWORD CHANGED SUCCESSFULLY");
            Warning(1);
            return;
        }

    strcpy(warning, "USER NOT FOUND");
    Warning(0);
}
