/*====================================Big Integer Calculator====================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 100

struct Digit;
typedef struct Digit* pointer; //define a pointer to Digit "node"

typedef struct Digit //define contents of each Digit "node"
{
    int value; //value of digit in the node Digit
    char sign; //sign of number to detect if positive or negative
    pointer next; //pointer to next Digit
    pointer previous; //pointer to previous Digit
};

typedef pointer LinkedList; //define a shortcut to linked list of Digits "Big Integer Number"
typedef pointer CurrentDigit; //define a shortcut to aimed position in liked list

LinkedList createDigit(int value) //creates head of specific value
{
    LinkedList l;
    l = (pointer)malloc(sizeof(struct Digit));
    l->next = NULL;
    l->previous = NULL;
    l->value = value;
    return l;
}

void enter(LinkedList list, int digitValue) //links first linked to new node "digit" at the end of first
{
    LinkedList digit;
    CurrentDigit link = list;
    if(digitValue == -1)
    {
        digit = createDigit(0);
        link->next = digit;
        digit->previous = link;
    }
    else
    {
        digit = createDigit(digitValue);
        while(link->next != NULL)
            link = link->next;
        link->next = digit;
        digit->previous = link;
    }
}

LinkedList enterAtTheHead(LinkedList head, int value) //links head to new node "digit" that set before head, so it will be the head exactly
{
    LinkedList digit = createDigit(value);
    while(head->previous!=NULL)
        head = head->previous;
    head->previous = digit;
    digit->next = head;
    return digit;
}

LinkedList fillLinkedList(char values[]) //function creates an Linked List and fill it with digits
{
    int i=0,value;
    LinkedList listOfDigits;

    if (values[0] == '-')
    {
        listOfDigits = createDigit((values[1]-'0'));
        listOfDigits->sign = '-';
        i = 2;
    }
    else
    {
        listOfDigits = createDigit((values[0]-'0'));
        i = 1;
    }

    while(values[i] != '\0')
    {
        value = (values[i]-'0');
        enter(listOfDigits,value);
        ++i;
    }
    return listOfDigits;
}

void deleteLinkedList(LinkedList l) //deletes given linked list from memory
{
    CurrentDigit nextDigit, previousDigit, flag1, flag2;
    nextDigit = l->next;
    previousDigit = l->previous;
    l->next = NULL;
    l->previous = NULL;
    while(nextDigit!=NULL && previousDigit!=NULL)
    {
        flag1 = nextDigit->next;
        flag2 = previousDigit->previous;
        free(nextDigit);
        free(previousDigit);
        nextDigit = flag1;
        previousDigit = flag2;
    }
}

LinkedList lastDigit(LinkedList l) //return last digit "least digit" in linked list that next of it equals NULL
{
    if(l->next==NULL)
        return l;
    else
        return lastDigit(l->next);
}

LinkedList firstDigit(LinkedList l) //return first digit"most digit" in linked list
{
    if(l->previous==NULL)
        return l;
    else
        return firstDigit(l->previous);
}

int countNoOfDigits(LinkedList l)  //counts number of digits "nodes" in linked list from last node!!
{
    int counter = 0;
    CurrentDigit last = l;
    last = lastDigit(last);
    while(last!=NULL)
    {
        ++counter;
        last = last->previous;
    }
    return counter;
}

int contrast(LinkedList l1, LinkedList l2) //contrast between two integers that head is first digit.
{
    if(l1!=NULL && l2!=NULL)
    {
        if(l1->value > l2->value)
        {
            l1 = firstDigit(l1);
            l2 = firstDigit(l2);
            return 1;
        }
        else if(l1->value < l2->value)
        {
            l1 = firstDigit(l1);
            l2 = firstDigit(l2);
            return -1;
        }
        else
            return contrast(l1->next,l2->next);
    }
    else
    {
        return 0;
    }
}

void printList(LinkedList List, FILE *fpt_out) //prints given linked list from head to last i.e |Head|->->|Last| both on screen and output file.
{
    CurrentDigit current = List;
    if(current->sign=='-')
    {
        printf("-");
        fprintf(fpt_out,"%c",'-');
    }
    while(current!=NULL)
    {
        printf("%d",current->value);
        fprintf(fpt_out,"%d",current->value);
        current = current->next;
    }
    fprintf(fpt_out,"\n");
}

LinkedList sub(LinkedList no1, LinkedList no2) //return the subtraction of two numbers and return the result as number represented of linked list
{
    int sub=0,flag = 0;
    LinkedList current1 = createDigit(no1->value), current2 = createDigit(no2->value), result;
    current1 = no1;
    current2 = no2;

    if(countNoOfDigits(current1)>countNoOfDigits(current2))
        while(countNoOfDigits(current1)!=countNoOfDigits(current2))
            current2 = enterAtTheHead(current2,0);
    else if(countNoOfDigits(current1)<countNoOfDigits(current2))
        while(countNoOfDigits(current2)!=countNoOfDigits(current1))
            current1 = enterAtTheHead(current1,0);

    if(contrast(current1,current2)==1)
    {
        current1 = lastDigit(current1);
        current2 = lastDigit(current2);
        sub = current1->value - current2->value;
        if(sub < 0)
        {
            sub += 10;
            flag = -1;
        }
        else
            flag = 0;

        result = createDigit(sub);
        current1 = current1->previous;
        current2 = current2->previous;

        while((current1->previous!=NULL) && (current2->previous!=NULL))
        {
            sub = current1->value - current2->value +flag;
            if(sub < 0)
            {
                sub += 10;
                flag = -1;
            }
            else
                flag = 0;

            result = enterAtTheHead(result,sub);
            current1 = current1->previous;
            current2 = current2->previous;
        }
        sub = current1->value - current2->value + flag;
        if(sub < 0)
            sub += 10;

        result = enterAtTheHead(result,sub);
        result = firstDigit(result);
        return result;
    }
    else if(contrast(current1,current2)==-1)
    {
        current1 = lastDigit(current1);
        current2 = lastDigit(current2);
        sub = current2->value - current1->value;
        if(sub < 0)
        {
            sub += 10;
            flag = -1;
        }
        else
            flag = 0;

        result = createDigit(sub);
        current1 = current1->previous;
        current2 = current2->previous;

        while((current1->previous!=NULL) && (current2->previous!=NULL))
        {
            sub = current2->value - current1->value +flag;
            if(sub < 0)
            {
                sub += 10;
                flag = -1;
            }
            else
                flag = 0;

            result = enterAtTheHead(result,sub);
            current1 = current1->previous;
            current2 = current2->previous;
        }
        sub = current2->value - current1->value + flag;
        if(sub < 0)
            sub += 10;

        result = enterAtTheHead(result,sub);
        result = firstDigit(result);
        result->sign = '-';
        return result;
    }
    else
    {
        result = createDigit(0);
        return result;
    }
}

LinkedList sum(LinkedList no1, LinkedList no2) //return the summation of two numbers and return the result as number represented of linked list
{
    int s1, s2, ones = 0, tens = 0, sum = 0;
    LinkedList current1 = createDigit(no1->value), current2 = createDigit(no2->value), result;
    current1 = no1;
    current2 = no2;

    if(countNoOfDigits(current1)>countNoOfDigits(current2))
        while(countNoOfDigits(current1)!=countNoOfDigits(current2))
            current2 = enterAtTheHead(current2,0);
    else if(countNoOfDigits(current1)<countNoOfDigits(current2))
        while(countNoOfDigits(current2)!=countNoOfDigits(current1))
            current1 = enterAtTheHead(current1,0);

    current1 = lastDigit(current1);
    current2 = lastDigit(current2);

    s1 = current1->value;
    s2 = current2->value;
    sum = s1 + s2;
    if (sum > 9)
    {
        ones = sum%10;
        tens = sum/10;
        result = createDigit(ones);
        if(current1->previous==NULL&&current2->previous==NULL)
        {
            result = firstDigit(result);
            return result;
        }
    }
    else
    {
        result = createDigit(sum);
        if(current1->previous==NULL&&current2->previous==NULL)
        {
            result = firstDigit(result);
            return result;
        }
    }
    current1 = current1->previous;
    current2 = current2->previous;

    while((current1->previous!=NULL) && (current2->previous!=NULL))
    {

        s1 = current1->value;
        s2 = current2->value;
        sum = s1 + s2 + tens;
        if (sum > 9)
        {
            ones = sum%10;
            tens = sum/10;
            result = enterAtTheHead(result,ones);
        }
        else
            result = enterAtTheHead(result,sum);
        current1 = current1->previous;
        current2 = current2->previous;
    }
    if(sum<=9)
        tens = 0;
    s1 = current1->value;
    s2 = current2->value;
    sum = s1 + s2 + tens;
    if (sum > 9)
    {
        ones = sum%10;
        tens = sum/10;
        result = enterAtTheHead(result,ones);
        result = enterAtTheHead(result,tens);
        result = firstDigit(result);
        return result;
    }
    else
    {
        result = enterAtTheHead(result,sum);
        result = firstDigit(result);
        return result;
    }
}

LinkedList mul(LinkedList no1, LinkedList no2) //return the multiplication of two numbers and return the result as number represented of linked list
{
    LinkedList current1=createDigit(no1->value), current2=createDigit(no2->value), temp, result = createDigit(0);
    int i = 0, counter = 1, mult = 0, carry = 0;
    current1 = no1;
    current2 = no2;

    current1 = lastDigit(current1);
    current2 = lastDigit(current2);


    mult = current1->value * current2->value;
    if(mult > 9)
    {
        carry = mult/10;
        mult = mult%10;
        temp = createDigit(mult);
    }
    else
        temp = createDigit(mult);
    current2 = current2->previous;
    while(current1!=NULL)
    {
        for(i = 2; i < counter; i++)
            temp = enterAtTheHead(temp,0);

        while(current2!=NULL)
        {
            mult = ((current1->value) * (current2->value)) + carry;
            if(mult>9)
            {
                carry = mult/10;
                mult = mult%10;
                if(current2->previous==NULL)
                {
                    temp = enterAtTheHead(temp,mult);
                    temp = enterAtTheHead(temp,carry);
                    carry = 0;
                    break;
                }
                else
                    temp = enterAtTheHead(temp,mult);
            }
            else
            {
                if(current2->previous==NULL)
                {
                    temp = enterAtTheHead(temp,mult);
                    carry = 0;
                    break;
                }
                else
                {
                    temp = enterAtTheHead(temp,mult);
                    carry = 0;
                }
            }

            current2 = current2->previous;
        }
        ++counter;
        result = sum(result,temp);
        result = firstDigit(result);
        deleteLinkedList(temp);
        temp = createDigit(0);
        current2 = lastDigit(current2);
        current1 = current1->previous;
    }
    result = firstDigit(result);
    return result;
}
/*
LinkedList longDivision(LinkedList no1, LinkedList no2) //return the division between two numbers and return linked list of the division.
{
    LinkedList current1=createDigit(no1->value), current2=createDigit(no2->value), temp, result = createDigit(0);
    LinkedList number, ten = createDigit(0);
    ten = enterAtTheHead(ten,1);
    int i = 0;
    current1 = no1;
    current2 = no2;

    number = createDigit(current1->value);
    current1 = current1->next;
    while(contrast(current1,current2)==-1 && current1!=NULL)
    {
        number = sum(mul(number,ten),current1);
        current1 = current1->next;
    }

    result->value = result->value + (number->value / current2->value);
    while(current1!=NULL)
    {
        result = enterAtTheHead(result,(number->value / current2->value));
        result = result->previous;
    }



    return result;
}
*/

LinkedList controller(LinkedList no1, LinkedList no2, int op) //controls the operations given on two numbers
{
    LinkedList result;
    char res = 'p';
    if(no1->sign == '-' && no2->sign == '-')
        res = 'n'; // n:negative
    else if(no1->sign != '-' && no2->sign != '-')
        res = 'p'; // p:positive
    else if(no1->sign == '-' && no2->sign != '-')
        res = 's'; // s:smaller
    else if(no1->sign != '-' && no2->sign == '-')
        res = 'g'; // g:greater

    switch(op)
    {
    case 1 : // 1:summation
        if(res=='p')
        {
            result = sum(no1,no2);
            return result;
        }
        else if(res=='g')
        {
            result = sub(no1,no2);
            return result;
        }
        else if(res=='s')
        {
            result = sub(no2,no1);
            return result;
        }
        else if(res=='n')
        {
            result = sum(no1,no2);
            result->sign='-';
            return result;
        }
    case 2 : // 2:subtraction
        if(res=='p')
        {
            result = sub(no1,no2);
            return result;
        }
        else if(res=='g')
        {
            result = sum(no1,no2);
            return result;
        }
        else if(res=='s')
        {
            result = sum(no1,no2);
            result->sign='-';
            return result;
        }
        else if(res=='n')
        {
            result = sub(no2,no1);
            return result;
        }
    case 3 : // 3:multiplication
        if(res=='p')
        {
            result = mul(no1,no2);
            return result;
        }
        else if(res=='g')
        {
            result = mul(no1,no2);
            result->sign='-';
            return result;
        }
        else if(res=='s')
        {
            result = mul(no1,no2);
            result->sign='-';
            return result;
        }
        else if(res=='n')
        {
            result = mul(no1,no2);
            return result;
        }
    /*case 4 : //4:division
        if(res=='p'){
        result = longDivision(no1,no2);
        return result;
    }else if(res=='g'){
        result = longDivision(no1,no2);
        result->sign='-';
        return result;
    }else if(res=='s'){
        result = longDivision(no1,no2);
        result->sign='-';
        return result;
    }else if(res=='n'){
        result = longDivision(no1,no2);
        return result;
    }*/
    default :
        return NULL;
    }

}

void menu(LinkedList no1, LinkedList no2) //menu list to run any operation
{
    int op; //op:operation
    LinkedList result;
    FILE *fpt_out;
    fpt_out = fopen("P1_1212508_QusayTaradeh_S2_Output.txt","w");
    while(1)
    {
        printf("\n==========MENU==========\n");
        printf("1.Summation\n");
        printf("2.Subtraction\n");
        printf("3.Multiplication\n");
        printf("4.Division\n");
        printf("5.Exit\n");

        printf("\nEnter number of operation you want it\n");
        scanf("%d",&op);

        if(op==5)
        {
            fclose(fpt_out);
            printf("\nThx<3\n");
            break;
        }
        else if(op==4)
            continue;

        result = controller(no1,no2,op);
        if(result!=NULL)
        {
            printf("\nResult : ");
            if(op==1)
                fprintf(fpt_out,"Summation result : ");
            else if(op==2)
                fprintf(fpt_out,"Subtraction result : ");
            else if(op==3)
                fprintf(fpt_out,"Multiplication result : ");
            //else
            //fprintf(fpt_out,"Division result : ");
            printList(result,fpt_out);
            printf("\n\n==========data to P1_1212508_QusayTaradeh_S2_Output document exported==========\n");
        }
        else
            printf("\nEnter valid number!\n");
    }
}

int main()
{
    LinkedList n1, n2;
    char fstNo[LENGTH]; //first number
    char secNo[LENGTH]; //second number

    FILE *fpt_in; //file of input data
    fpt_in=fopen("P1_1212508_QusayTaradeh_S2_Input.txt","r");
    fscanf(fpt_in,"%s",fstNo); // 62233344445555566113
    fscanf(fpt_in,"%s",secNo); // 67777777888888889992
    printf("==========data from P1_1212508_QusayTaradeh_S2_Input document uploaded==========\n");
    printf("First  : %s\n",fstNo);
    printf("Second : %s\n",secNo);
    fclose(fpt_in);

    n1 = fillLinkedList(fstNo); //first number represented in linked list
    n2 = fillLinkedList(secNo); //second number represented in linked list
    menu(n1,n2);


    printf("\n\n");
    return 0;
}

