#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;

#define MAXSIZE 60

/**
 * 模板顺序栈定义
 */
template <typename T>
struct SqStack {
    T data[MAXSIZE];
    int top;

    // 初始化
    void init() {
        top = -1;
    }

    // 判空
    bool empty() const {
        return top == -1;
    }

    // 入栈
    bool push(T x) {
        if (top >= MAXSIZE - 1)
            return false;
        data[++top] = x;
        return true;
    }

    // 出栈（不带参数版本）
    bool pop() {
        if (empty())
            return false;
        top--;
        return true;
    }

    // 出栈（带参数版本）
    bool pop(T &x) {
        if (empty())
            return false;
        x = data[top--];
        return true;
    }

    // 取栈顶
    bool getTop(T &x) const {
        if (empty())
            return false;
        x = data[top];
        return true;
    }
};

/**
 * 中缀表达式转后缀表达式
 */
void trans(char *exp, char postexp[]) {
    char e;
    // 声明一个使用 char 类型的顺序栈
    SqStack<char> Optr;
    Optr.init();

    // 下标
    int index = 0;

    while (*exp != '\0') {
        switch (*exp) {
            // 左括号，直接进栈
            case '(':
                Optr.push(*exp);
                exp++;
                break;

            // 右括号，入栈，循环，直至遇到左括号
            case ')':
                Optr.pop(e);
                while (e != '(') {
                    postexp[index++] = e;
                    Optr.pop(e);
                }
                exp++;
                break;

            // '+'、'-'
            case '+':
            case '-':
                // 栈不空
                while (!Optr.empty()) {
                    Optr.getTop(e);
                    if (e == '+' || e == '-' || e == '*' || e == '/') {
                        // 优先级相同或更高，直接出栈栈顶元素
                        postexp[index++] = e;
                        Optr.pop(e);
                    } else {
                        break;
                    }
                }
                // 直接进栈当前运算符
                Optr.push(*exp);
                exp++;
                break;

            // '*'、'/'
            case '*':
            case '/':
                // 栈不空，循环
                while (!Optr.empty()) {
                    Optr.getTop(e);
                    if (e == '*' || e == '/') {
                        // 优先级相同或更高，直接出栈栈顶元素
                        postexp[index++] = e;
                        Optr.pop(e);
                    } else {
                        break;
                    }
                }
                // 直接进栈当前运算符
                Optr.push(*exp);
                exp++;
                break;

            // 数字，直接输出
            default:
                while (isdigit(*exp)) {
                    postexp[index++] = *exp;
                    exp++;
                }
                // 用.标志操作数结束
                postexp[index++] = '.';
        }
    }

    // 处理完exp后，栈中还有元素，出栈
    while (!Optr.empty()) {
        Optr.getTop(e);
        Optr.pop(e);
        if (e != '(') {
            // 直接出栈栈顶元素
            postexp[index++] = e;
        }
    }

    // 给 postexp 加上结束符'@'
    postexp[index++] = '@';
    // 确保字符串正确结束
    postexp[index] = '\0';
}

// 后缀表达式求值
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
            if (!st.pop(b)) { cerr << "错误：缺少操作数\n"; exit(1); }
            if (!st.pop(a)) { cerr << "错误：缺少操作数\n"; exit(1); }

            int res = 0;
            switch (ch) {
                case '+': res = a + b; break;
                case '-': res = a - b; break;
                case '*': res = a * b; break;
                case '/':
                    if (b == 0) {
                        cerr << "错误：除数为0\n";
                        exit(1);
                    }
                    res = a / b; // C++ 默认向0取整
                    break;
            }
            st.push(res);
        }
    }

    int result;
    if (st.getTop(result)) {
        return result;
    } else {
        cerr << "错误：表达式无效或结果为空\n";
        exit(1);
    }
}

int main() {
    char exp[MAXSIZE], postexp[MAXSIZE];
    cout << "请输入中缀表达式：" << endl;
    cin.getline(exp, MAXSIZE);

    trans(exp, postexp);
    cout << "后缀表达式为：" << postexp << endl;

    int result = evaluatePostfix(postexp);
    cout << "计算结果为：" << result << endl;

    return 0;
}