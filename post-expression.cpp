#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;

#define MAXSIZE 60

/**
 * ģ��˳��ջ����
 */
template <typename T>
struct SqStack {
    T data[MAXSIZE];
    int top;

    // ��ʼ��
    void init() {
        top = -1;
    }

    // �п�
    bool empty() const {
        return top == -1;
    }

    // ��ջ
    bool push(T x) {
        if (top >= MAXSIZE - 1)
            return false;
        data[++top] = x;
        return true;
    }

    // ��ջ�����������汾��
    bool pop() {
        if (empty())
            return false;
        top--;
        return true;
    }

    // ��ջ���������汾��
    bool pop(T &x) {
        if (empty())
            return false;
        x = data[top--];
        return true;
    }

    // ȡջ��
    bool getTop(T &x) const {
        if (empty())
            return false;
        x = data[top];
        return true;
    }
};

/**
 * ��׺���ʽת��׺���ʽ
 */
void trans(char *exp, char postexp[]) {
    char e;
    // ����һ��ʹ�� char ���͵�˳��ջ
    SqStack<char> Optr;
    Optr.init();

    // �±�
    int index = 0;

    while (*exp != '\0') {
        switch (*exp) {
            // �����ţ�ֱ�ӽ�ջ
            case '(':
                Optr.push(*exp);
                exp++;
                break;

            // �����ţ���ջ��ѭ����ֱ������������
            case ')':
                Optr.pop(e);
                while (e != '(') {
                    postexp[index++] = e;
                    Optr.pop(e);
                }
                exp++;
                break;

            // '+'��'-'
            case '+':
            case '-':
                // ջ����
                while (!Optr.empty()) {
                    Optr.getTop(e);
                    if (e == '+' || e == '-' || e == '*' || e == '/') {
                        // ���ȼ���ͬ����ߣ�ֱ�ӳ�ջջ��Ԫ��
                        postexp[index++] = e;
                        Optr.pop(e);
                    } else {
                        break;
                    }
                }
                // ֱ�ӽ�ջ��ǰ�����
                Optr.push(*exp);
                exp++;
                break;

            // '*'��'/'
            case '*':
            case '/':
                // ջ���գ�ѭ��
                while (!Optr.empty()) {
                    Optr.getTop(e);
                    if (e == '*' || e == '/') {
                        // ���ȼ���ͬ����ߣ�ֱ�ӳ�ջջ��Ԫ��
                        postexp[index++] = e;
                        Optr.pop(e);
                    } else {
                        break;
                    }
                }
                // ֱ�ӽ�ջ��ǰ�����
                Optr.push(*exp);
                exp++;
                break;

            // ���֣�ֱ�����
            default:
                while (isdigit(*exp)) {
                    postexp[index++] = *exp;
                    exp++;
                }
                // ��.��־����������
                postexp[index++] = '.';
        }
    }

    // ������exp��ջ�л���Ԫ�أ���ջ
    while (!Optr.empty()) {
        Optr.getTop(e);
        Optr.pop(e);
        if (e != '(') {
            // ֱ�ӳ�ջջ��Ԫ��
            postexp[index++] = e;
        }
    }

    // �� postexp ���Ͻ�����'@'
    postexp[index++] = '@';
    // ȷ���ַ�����ȷ����
    postexp[index] = '\0';
}

// ��׺���ʽ��ֵ
int evaluatePostfix(char *postexp) {
    SqStack<int> st;
    st.init();
    int num = 0;
    bool hasDigits = false;

    while (*postexp != '@') {
        char ch = *postexp++;

        if (isdigit(ch)) {
            num = num * 10 + (ch - '0');
            hasDigits = true;
        } else if (ch == '.') {
            if (hasDigits) {
                st.push(num);
                num = 0;
                hasDigits = false;
            }
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            int b, a;
            if (!st.pop(b)) { cerr << "����ȱ�ٲ�����\n"; exit(1); }
            if (!st.pop(a)) { cerr << "����ȱ�ٲ�����\n"; exit(1); }

            int res = 0;
            switch (ch) {
                case '+': res = a + b; break;
                case '-': res = a - b; break;
                case '*': res = a * b; break;
                case '/':
                    if (b == 0) {
                        cerr << "���󣺳���Ϊ0\n";
                        exit(1);
                    }
                    res = a / b; // C++ Ĭ����0ȡ��
                    break;
            }
            st.push(res);
        }
    }

    int result;
    if (st.getTop(result)) {
        return result;
    } else {
        cerr << "���󣺱��ʽ��Ч����Ϊ��\n";
        exit(1);
    }
}

int main() {
    char exp[MAXSIZE], postexp[MAXSIZE];
    cout << "��������׺���ʽ��" << endl;
    cin.getline(exp, MAXSIZE);

    trans(exp, postexp);
    cout << "��׺���ʽΪ��" << postexp << endl;

    int result = evaluatePostfix(postexp);
    cout << "������Ϊ��" << result << endl;

    return 0;
}