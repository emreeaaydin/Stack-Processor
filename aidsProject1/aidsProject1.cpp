#include <cstdio>
#include <iostream>
using namespace std;

// ....... Node and List definitions .....
struct Node {
    char value;
    explicit Node(char v) : value(v), next(nullptr) {}
    Node* next;
};


struct List {
    Node* head;
    List() : head(nullptr) {}

    void push_front(char c) {
        Node* newNode = new Node(c);
        newNode->next = head;
        head = newNode;
    }

    char pop_front() {
        if (!head) return '\0';
        char val = head->value; // head -> node('a') then val = a
        Node* temp = head;
        head = head->next; // The head of list shifts to the next node
        delete temp;
        return val; // value we deleted with "delete temp;" is returned
    }

    bool is_empty() const {
        return head == nullptr; // if no element in the head so nullptr == true
    }

    List* clone() const {
        List* copy = new List();
        copy->head = clone_nodes(head); // the chain of node is connected to the head of the new list.
        return copy;
    }

    Node* clone_nodes(Node* node) const {
        if (!node) return nullptr;
        Node* newNode = new Node(node->value); // node content is the same as the value of the copied Node (value)
        newNode->next = clone_nodes(node->next);
        return newNode;
    }

    void print() const {
        print_recursive(head);
        putchar('\n');
    }

    void print_recursive(Node* node) const {
        if (!node) return;
        putchar(node->value);
        print_recursive(node->next);
    }

    bool is_number() const { // if all the characters are numbers (or if there is one '-') returns true
        return is_number_recursive(head, false);
    }

    bool is_number_recursive(Node* node, bool usedMinus) const { 
        if (!node) return true;
        if (!node->next && node->value == '-') { // last node and the value is -
            return !usedMinus; // return true
        }
        if (node->value < '0' || node->value > '9') {
            return false;
        }
        return is_number_recursive(node->next, usedMinus);
    }

    int to_number() const {
        if (!is_number()) return 0;
        long long val = to_number_recursive(head, 1LL); 
        // overflow protection return 0 outside the 32bit limits.
        if (val > 2147483647LL || val < -2147483648LL) {
            return 0;
        }
        return is_negative() ? -static_cast<int>(val) : static_cast<int>(val); // if there is '-' at the end of the list, the number is negative
                                                                               // otherwise, it turns positive as it is
    }

    long long to_number_recursive(Node* node, long long power) const { // read the list in reverse order and converts it to the number
        if (!node) return 0;
        if (!node->next && node->value == '-') return 0; // if the last thing is '-' it is just a sign not effect as number
        //if power * 10 is overflow stop the increase and turn 0 
        if (power > 922337203685477580LL) return 0;
        return (node->value - '0') * power + to_number_recursive(node->next, power * 10LL);
    }

    bool is_negative() const {
        return is_negative_recursive(head);
    }

    bool is_negative_recursive(Node* node) const {
        if (!node) return false;
        if (!node->next && node->value == '-') { // If this node is the last node and value '-', this is a negative number.
            return true;
        }
        return is_negative_recursive(node->next); // calls every node till the last one
    }

    void from_number(int n) {
        head = nullptr;
        if (n == 0) { // special case 
            push_front('0'); // 0 directly added because it has only one digit
            return;
        }
        bool neg = (n < 0); // negative number check
        if (neg) n = -n; // makes the number positive
        from_number_recursive(n);
        if (neg) push_front('-'); // after recursive if still neg then - pushes front
    }

    void from_number_recursive(int n) {
        if (n == 0) return; // if no digit left stop
        from_number_recursive(n / 10); // with dividing by 10 we can switch to next digit
        push_front(char((n % 10) + '0')); // the rightest digit finding with n%10 and transforms to ascii with addition of '0' then push front
    }

    bool is_truthy() const {
        if (!head) return false;
        if (!head->next && head->value == '0') return false;
        return true;
    }

    bool has_dash_at_end() const {
        return has_dash_recursive(head);
    }

    bool has_dash_recursive(Node* node) const {
        if (!node) return false;
        if (!node->next) {
            return (node->value == '-'); // The value of the last node is checked: if the last '-' ->true, if not, false
        }
        return has_dash_recursive(node->next);
    }

    void remove_last() {
        head = remove_last_recursive(nullptr, head);
    }

    Node* remove_last_recursive(Node* prev, Node* curr) {
        if (!curr) return nullptr;
        if (!curr->next) {
            delete curr; // delets the last node
            if (prev) prev->next = nullptr; // cut the connection of the deleted node
            return nullptr; // saves the remaning part
        }
        curr->next = remove_last_recursive(curr, curr->next);
        return curr;
    }

    void append_dash() {
        append_dash_recursive(head);
    }

    void append_dash_recursive(Node* node) {
        if (!node->next) {
            node->next = new Node('-'); // if there is no node remaining add new '-' node as the last and new node
        }
        else {
            append_dash_recursive(node->next); // try till find the last node
        }
    }

    void append_list(List* other) {
        if (!head) {
            head = other->clone()->head;
        }
        else {
            append_list_recursive(head, other->clone()->head);
        }
    }

    void append_list_recursive(Node* target, Node* addition) {
        if (!target->next) {
            target->next = addition;
        }
        else {
            append_list_recursive(target->next, addition);
        }
    }

    ~List() {
        destroy_recursive(head);
    }

    void destroy_recursive(Node* node) {
        if (!node) return;
        destroy_recursive(node->next);
        delete node;
    }

    static bool is_all_zero_ignoring_dash(Node* node) {
        if (!node) return true; // empty = 0 , thats what we want
        if (!node->next && node->value == '-') return true; // last node is '-' then this is also 0, so true
        if (node->value != '0') return false; // 1,5,7,8... !=0 -> false
        return is_all_zero_ignoring_dash(node->next);
    }

    static int length_ignoring_dash(Node* node) { // XXXX
        if (!node) return 0;
        if (!node->next && node->value == '-') return 0;
        return 1 + length_ignoring_dash(node->next);
    }

    static bool is_negative_big(Node* node) {  // XXXXXX
        if (!node) return false;
        if (!node->next && node->value == '-') return true;
        return is_negative_big(node->next);
    }

    static Node* nth_from_end_ignoring_dash(Node* node, int& counter, int target) { // XXXX
        if (!node) return nullptr;
        Node* res = nth_from_end_ignoring_dash(node->next, counter, target);
        if (!node->next && node->value == '-') return res;  
        if (counter == target) return node;
        counter++;
        return res;
    }

    static char get_digit_from_end(Node* node, int idx) { // XXXX
        int counter = 0;
        Node* found = nth_from_end_ignoring_dash(node, counter, idx);
        return found ? found->value : '0';
    }

    static int compare_digits_recursive(Node* a, Node* b, int idx, bool bothNeg) { // XXXXX
        if (idx < 0)
            return 0; // all digits are equal
        char da = get_digit_from_end(a, idx);
        char db = get_digit_from_end(b, idx);
        if (da < db)
            return bothNeg ? +1 : -1;
        if (da > db)
            return bothNeg ? -1 : +1;
        return compare_digits_recursive(a, b, idx - 1, bothNeg);
    }

    static void build_natural(Node* node, char* buf, int& pos) { // XXXX
        if (!node) return;
        if (!node->next && node->value == '-')
            return; // son düğüm '-' ise yazma
        build_natural(node->next, buf, pos);
        buf[pos++] = node->value;
    }

    // the idx start index increases as long as both arrays are equal
    static int compare_strings(const char* s1, const char* s2, int idx) { // XXXX
        if (s1[idx] == '\0' && s2[idx] == '\0')
            return 0;
        if (s1[idx] < s2[idx])
            return -1;
        if (s1[idx] > s2[idx])
            return 1;
        return compare_strings(s1, s2, idx + 1);
    }


    struct CharNode {
        char c;
        CharNode* next;
        explicit CharNode(char ch) : c(ch), next(nullptr) {}
    };

    // Recursively append a node to the tail of a CharNode list. '1' -> '2' + ->'3'
    static CharNode* append_to_tail(CharNode* head, CharNode* nodeToAppend) {
        if (!head) return nodeToAppend;
        head->next = append_to_tail(head->next, nodeToAppend);
        return head;
    }

    // recursive function to build the natural representation of a list
    static CharNode* build_natural_node(Node* node) {
        if (!node || (!node->next && node->value == '-')) // if the last node and value is '-'
            return nullptr;
        CharNode* sub = build_natural_node(node->next); // recursion to next node
        CharNode* curr = new CharNode(node->value); // creates a CHARNode using the value of the node
        return append_to_tail(sub, curr); // at the end of the sub chain, curr is added
    }

    // recursively create a complete copy of a CharNode list
    static CharNode* copy_char_list(CharNode* node) {
        if (!node) return nullptr;
        CharNode* newNode = new CharNode(node->c);
        newNode->next = copy_char_list(node->next);
        return newNode;
    }

    // recursive function that skips leading '0' characters
    static CharNode* skip_leading_zeros(CharNode* node) {
        if (!node) return nullptr;
        if (node->c == '0')
            return skip_leading_zeros(node->next); // it progresses until an un-zero step is found
        // from now on copy the list and create a new list
        CharNode* newHead = new CharNode(node->c); // when unzero found
        newHead->next = copy_char_list(node->next);
        return newHead;
    }

    // recursively count the length of a CharNode list
    static int char_list_length(CharNode* node) {
        if (!node) return 0;
        return 1 + char_list_length(node->next); // +1`i index saydirmak gibi dusun 
    }

    // recursively compare two CharNode lists
    static int compare_char_lists(CharNode* a, CharNode* b) {
        if (!a && !b) return 0; // both of are ended then equal =0
        if (!a) return -1; // a ends first the a is smaler = -1
        if (!b) return 1;
        if (a->c < b->c) return -1; // a<b
        if (a->c > b->c) return 1; // b>a
        return compare_char_lists(a->next, b->next);
    }

    // recursively delete (free) a CharNode list.
    static void delete_char_list(CharNode* node) {
        if (!node) return;
        delete_char_list(node->next); // it goes back to the end and deletes it when it turns back
        delete node;
    }

    static int internal_big_compare(const List* A, const List* B) {

        if (List::is_all_zero_ignoring_dash(A->head) &&
            List::is_all_zero_ignoring_dash(B->head))
        {
            return 0;
        }// 0, -, 0-> - = equal

        CharNode* rawA = build_natural_node(A->head); // Functions held with the opposite turn are translated
        CharNode* rawB = build_natural_node(B->head);

        CharNode* natA = skip_leading_zeros(rawA); // '0' -> '0' -> '1' == '1'
        CharNode* natB = skip_leading_zeros(rawB);

        int lenA = char_list_length(natA); // the more digited number is bigger
        int lenB = char_list_length(natB);

        int result = 0;
        if (lenA < lenB) result = -1;
        else if (lenA > lenB) result = 1;
        else result = compare_char_lists(natA, natB); // If the lengths are equal compare the numbers in order :

        // delete to prevent memory leaks.
        delete_char_list(rawA);
        delete_char_list(rawB);
        delete_char_list(natA);
        delete_char_list(natB);

        return result;
    }

    // large-small-equal control
    static int big_compare(const List* A, const List* B) {
        bool negA = A->is_negative(); // 12- negA true
        bool negB = B->is_negative();
        if (negA && !negB)
            return -1; // neg smaller than pos (based on A)
        if (!negA && negB)
            return +1; // pos bigger than neg
        int cmp = internal_big_compare(A, B); // if they have same sign 
        if (negA && negB)
            return -cmp;  // both are neg then compare get reversed
        return cmp;
    }
};

// ....... Stack .........
struct StackNode {
    List* data;
    StackNode* next;
    explicit StackNode(List* l) : data(l), next(nullptr) {}
};

struct Stack {
    StackNode* top; // shows the node on top of the stack
    Stack() : top(nullptr) {} // top is empty then stack is empty

    void push(List* l) {
        StackNode* newNode = new StackNode(l);
        newNode->next = top;
        top = newNode;
    }

    List* pop() {
        if (!top) return nullptr;
        StackNode* temp = top;
        List* result = top->data;
        top = top->next;
        delete temp;
        return result;
    }

    List* peek() const {
        if (!top) return nullptr;
        return top->data;
    }

    List* get(int index) const { // index == 0 is where we wanted to be
        return get_recursive(top, index); 
    }

    List* get_recursive(StackNode* node, int index) const {
        if (!node) return nullptr; // if the stack is empty or more than the desired depth
        if (index == 0) return node->data; // return List*data as node ->data
        return get_recursive(node->next, index - 1);
    }

    void swap_top_two() {
        if (!top || !top->next) return;
        StackNode* first = top;
        StackNode* second = top->next;
        first->next = second->next; // A is now connected to the node below B.
        second->next = first; // B's Next is A -> so the B is on top.
        top = second; // the new stack of the pile is B.
    }

    void print() const {
        print_recursive(top, 0);
    }

    void print_recursive(StackNode* node, int depth) const {
        if (!node) return;
        print_recursive(node->next, depth + 1); // we go to the lowest node first because we want to print stack from the bottom upwards
        printf("%d: ", depth); // the depth of the current knot is print on the screen
        node->data->print(); // the list* object on the node is printed
    }

    ~Stack() {
        destroy_recursive(top);
    }

    void destroy_recursive(StackNode* node) {
        if (!node) return;
        destroy_recursive(node->next);
        delete node->data;
        delete node;
    }
};

// ......execute().......
void execute(const char* program, const char* input, int& ip, Stack& stack, int& input_pos)
{
    if (program[ip] == '\0') return;

    char instr = program[ip];

    switch (instr) {
    case '\'': { // push an empty list on the stack
        stack.push(new List());
        break;
    }
    case ',': { // pop a list from the stack
        List* removed = stack.pop();
        if (removed) delete removed;
        break;
    }
    case ':': { // put a copy of the list on the top of the stack (adds to the stack as a separate copy, it does not add elements to the same list.)
        List* top = stack.peek();
        if (top) 
            (top->clone()); // this creates a new list and is a copy of the old one
        break;
    }
    case ';': { // swap the places of the list on the top of the stack and the list directly below it
        stack.swap_top_two();
        break;
    }
    case '@': { // It takes a number from the top of the Stack -> finds the following list up to that number -> Takes its copy and adds it to the top.
        List* a = stack.pop();
        if (a && a->is_number()) {
            int index = a->to_number();
            List* tgt = stack.get(index);
            if (tgt) {
                stack.push(tgt->clone());
            }
        }
        if (a) delete a;
        break;
    }
    case '&': { // print the contents of the stack
        stack.print();
        break;
    }
    case '.': {
        // the command takes a character from the Input series and adds it to the top of the list above Stack; If the stack is empty, it creates a new list.
        if (input[input_pos] != '\0') { // input_pos shows which character we are currently
            char c = input[input_pos++]; // first holds the first char then pos +1 then holds second 
            List* top = stack.peek(); // only shows the top list no delete
            if (!top) {
                top = new List();
                stack.push(top); // add to Stack with Push()
            }
            top->push_front(c);
        }
        break;
    }

    case '>': { // The first character is taken from the list at the top of Stack and written on the screen. Then the list is completely deleted.
        List* top = stack.pop();
        if (top) {
            char c = top->pop_front(); // val from pop_front becomes c here
            if (c != '\0') putchar(c);
            delete top;
        }
        break;
    }
    case '-': { // negation: at the end of the list '-' if there is -> it removes it. otherwise -> end '-' adds.
        
        List* top = stack.peek(); // list on top of stack
        if (!top) break; // if STACK is empty
       
        if (!top->head) { // if THE LIST is empty
            top->push_front('-');
        }
        else if (top->has_dash_at_end()) {
            top->remove_last(); // if there is minus at the end
        }
        else { // The list is not empty, there is no '-' at the end
            top->append_dash();
        }
        break;
    }
    case '^': { // if you have '-' at the end of the node chain, it only deletes it. it doesn't do anything else.
        List* top = stack.peek();
        if (top && !top->is_empty() && top->has_dash_at_end()) {
            top->remove_last();
        }
        break;
    }
    case '$': { // split: detach the first character from the top list and push it onto the stack
        List* topList = stack.peek();
        if (topList && !topList->is_empty()) {
            char c = topList->pop_front();
            List* newList = new List();
            newList->push_front(c);
            stack.push(newList);
        }
        break;
    }
    case '#': { // pop list A off the stack; append A to the end of the list at the top of the stack
        List* a = stack.pop();
        List* t = stack.peek();
        if (a && t) {
            if (!t->head) { // t empty
                t->head = a->clone()->head; // the chain (a) is cloned and connected directly to the head of t
            }
            else {
                t->append_list(a); // if the target chain is not empty a is added to the end of the chain t
            }
        }
        if (a) delete a;
        break;
    }
    case '[': { // pop list A off the stack; put the number equal to the ASCII number of the first character from list A on the stack
        List* a = stack.pop();
        if (a && !a->is_empty()) {
            int ascii = a->pop_front(); // values from a taken as intiger
            List* res = new List();
            res->from_number(ascii); // with from_number() ints becomes digit values (??)
            stack.push(res);
        }
        if (a) delete a;
        break;
    }
    case ']': { // opposite of [
        List* a = stack.pop(); // number-chain is taken from the top of stack ('6' -> '5')
        if (a && a->is_number()) {
            int ascii = a->to_number(); // ('6' -> '5') = 65
            if (ascii >= 0 && ascii <= 255) {
                List* res = new List();
                res->push_front((char)ascii); // (65)ascii = 'A'
                stack.push(res);
            }
            else {
                stack.push(new List()); // if the list invalid
            }
        }
        else {
            stack.push(new List()); // if the list is number
        }
        if (a) delete a;
        break;
    }
    case '=': { // It takes two lists from the Stack, compares them: if it is equal, it creates a list containing 1, if not, and adds to Stack.
        List* a = stack.pop(); // upper list
        List* b = stack.pop(); // one lower list


        if (a && b && a->is_all_zero_ignoring_dash(a->head) && b->is_all_zero_ignoring_dash(b->head)) { // if the list only contains 0 and - then return true
            List* res = new List();
            res->push_front('1'); // if the - and 0 is fixed then return '1'
            stack.push(res);
        }
        else if (a && b) { // no 0 and - just number comparison
            if (a->is_number() && b->is_number()) {
                int cmp = List::big_compare(b, a); // b is comparing with a
                List* res = new List();
                res->push_front((cmp == 0) ? '1' : '0'); 
                stack.push(res);
            }
        }
        if (a) delete a;
        if (b) delete b;
        break;
    }


    case '<': { // pop number A off the stack, pop number B off the stack, if B < A put number 1 on the stack, otherwise put number 0 on the stack
        List* a = stack.pop();
        List* b = stack.pop();

        if (a && b && a->is_all_zero_ignoring_dash(a->head) && b->is_all_zero_ignoring_dash(b->head)) {
            // if both are zero, the result is '0' -> false
            List* res = new List();
            res->push_front('0'); // 0 !< 0 
            stack.push(res);
        }
        else if (a && b) {
            if (a->is_number() && b->is_number()) {
                int cmp = List::big_compare(b, a);
                List* res = new List();
                res->push_front((cmp < 0) ? '1' : '0'); // if b<a -> cmp<0 then result '1'
                stack.push(res);
            }
        }
        if (a) delete a;
        if (b) delete b;
        break;
    }

    case '!': { // if the list is empty or only contains '0', it turns '1', otherwise '0'
        List* top = stack.pop();
        List* result = new List();
        if (!top || top->is_empty() || (!top->head->next && top->head->value == '0')) {
            result->push_front('1'); // this is false but logical reverse of 0 is 1
        }
        else {
            result->push_front('0'); // other all senerios are true. !true = false -> 0
        }
        if (top) delete top;
        stack.push(result); // after deleting of list new result 1 or 0 pushes to new list
        break;
    }
    case '~': { // It turns the program meter(ip) into a list as a number and adds it to stack
        List* r = new List();
        r->from_number(ip); // onvert the number of positions (ip) to the list
        stack.push(r);
        break;
    }
    case '?': {
        List* t = stack.pop(); // target
        List* w = stack.pop(); // condition
        if (t && t->is_number() && w && w->is_truthy()) {
            int target = t->to_number(); // t list converted to a number this will ip value
            if (target >= 0 && program[target] != '\0') { 
                delete t;
                delete w;
                ip = target; // ip updated program will work with target after on
                execute(program, input, ip, stack, input_pos);
                return;
            }
        }
        if (t) delete t;
        if (w) delete w;
        break;
    }
    default: {
        List* top = stack.peek();
        if (!top) { // if stack is empty
            top = new List();
            stack.push(top);
        }
        top->push_front(instr); // adds the "input" in list on stack
        break;
    }
    }

    ip++;
    execute(program, input, ip, stack, input_pos);
}

int main() {
    char program[20001]; // for commands
    char inputData[20001]; 
    int progIndex = 0, inputIndex = 0;
    int c;
    int phase = 0; // 0: program read, 1: inputData read

    // in a single cycle, we first read the program and then input data.
    while ((c = getchar()) != EOF) {
        // if the gap or a new line character comes:
        if (c == ' ' || c == '\n') {
            if (phase == 0) { // If the program is still reading the command part
                program[progIndex] = '\0'; 
                phase = 1; // prog input ended
                inputIndex = 0;
            }
            continue;
        }
        if (phase == 0) {
            if (progIndex < 20000) { 
                program[progIndex++] = c;
            }
        }
        else {
            if (inputIndex < 20000) {  
                inputData[inputIndex++] = c; // write the input series input and increase index
            }
        }
    }
    
    if (phase == 0)
        program[progIndex] = '\0';
    else
        inputData[inputIndex] = '\0';

    Stack s;
    int input_pos = 0;
    int ip = 0;
    execute(program, inputData, ip, s, input_pos);
    return 0;
}
