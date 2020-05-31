#ifndef MODVECTOR_H
#define MODVECTOR_H
#include <iostream>
#include <string>
#include <ostream>
using namespace std;
//const int nullptr = 0;

const int DEF_CAP = 16;
template<typename T>
class modvector
{
    //friend ostream& operator<<(ostream& out,const modvector<T>& obj);
    template<typename T2>
    friend ostream & operator<<(ostream& out,const modvector<T2>& obj);
    public:
        modvector();
        modvector(T obj,int len);
        modvector(modvector<T>& obj);
        virtual ~modvector();

        T& operator[](int index);
        void push_back(const T& obj);
        T pop_back();
        void clear();
        /*下面这些函数都要求返回迭代器类型，暂时不会*/
        /*
        void begin();
        void end();
        void cbegin();
        void cend();*/

        void erase(int);

        int size()const{return m_arrylen;}
        modvector<T>& operator=(const modvector<T>& obj);

    private:
        T* m_ptrspace;
        int m_capacity;
        int m_arrylen;
};

/** 默认构造函数*/
template<typename T>
modvector<T>::modvector()
{
    m_capacity = DEF_CAP;
    m_arrylen = 0;
    m_ptrspace = new T[m_capacity];
}

/** 带参构造函数,构造len个obj对像*/
template<typename T>
modvector<T>::modvector(T obj,int len)
{
    if(len < DEF_CAP)
    {
        m_capacity = DEF_CAP;
    }
    else
    {
        m_capacity = 2 * DEF_CAP;
    }

    m_arrylen = len;
    m_ptrspace = new T[m_capacity];

    for(int i = 0;i<len;i++)
    {
        m_ptrspace[i] = obj;
    }
}

/** 析构函数，释放内存*/
template<typename T>
modvector<T>::~modvector()
{
    if(m_ptrspace != nullptr)
    {
        delete[] m_ptrspace;
        m_ptrspace = nullptr;
    }
    else
    {
        cout << "指针为空"<<endl;
    }
}

/**重载输出运算符*/
template<typename T>
ostream& operator<<(ostream& out,const modvector<T>& obj)
{
    for(int i = 0;i<obj.size();i++)
    {
        out<<obj[i]<<',';
    }
    out<<endl;
    return out;
}
/**赋值运算符重载*/
template<typename T>
modvector<T>& modvector<T>::operator=(const modvector<T>& obj)
{
    if(this == &obj) return *this;

    if(nullptr != m_ptrspace)
    {
        delete[] m_ptrspace;
        m_ptrspace = nullptr;
    }
    m_capacity = obj.m_capacity;
    m_arrylen = obj.m_arrylen;
    m_ptrspace = new T[m_capacity];

    /*担心浅拷贝*/
    //memcpy(m_ptrspace,obj.m_ptrspace,sizeof(T)*m_arrylen);
    for(int i = 0;i<obj.size();i++)
    {
        m_ptrspace[i] = obj.m_ptrspace[i];
    }
    return *this;
    /*
    if(m_arrylen< obj.m_arrylen)
    {
        m_capacity = obj.m_capacity;
        this->m_arrylen = obj.m_arrylen;

        T* ptrNew = new T[m_capacity];
        delete [] this->m_ptrspace;
        this->m_ptrspace = ptrNew;
    }
    memcpy(this->m_ptrspace,obj.m_ptrspace,sizeof(T)*this->m_arrylen);
    return *this;*/
}

/**拷贝构造函数,在初始化的时候调用，所以不用释放内存*/
template<typename T>
modvector<T>::modvector(modvector<T>& obj)
{
    m_arrylen = obj.m_arrylen;
    m_capacity = obj.m_capacity;
    m_ptrspace = new T[m_capacity];

    for(int i = 0;i<obj.size();i++)
    {
        m_ptrspace[i] = obj.m_ptrspace[i];
    }
    memcpy(m_ptrspace,obj.m_ptrspace,obj.m_arrylen * sizeof(T));
}

/**[] 运算符重载，因为要可以作为左值，所以不能加const*/
template<typename T>
T& modvector<T>::operator[](int index)
{
    if(index <0 ||index >= m_arrylen)
    {
        cerr<<"访问越界"<<endl;
        //return;
    }
    return m_ptrspace[index];
}

/**拷贝，没有做内存大小的检查
void modvector::veccopy(T& dst,const T& src)
{
    for(int i = 0;i<src.size();i++)
    {
        dst[i] = src[i];
    }
}*/

/**压栈操作*/
template<typename T>
void modvector<T>::push_back(const T& obj)
{
    if(nullptr == m_ptrspace)
    {
       m_capacity = DEF_CAP;
       m_arrylen = 0;
       m_ptrspace = new T[m_capacity];
    }

    if(m_arrylen == m_capacity)
    {
        T* ptrNewObj = new T[2*m_capacity + 1];

        for(int i = 0;i<m_arrylen;i++)
        {
            ptrNewObj[i] = m_ptrspace[i];
        }
        //memcpy(ptrNewObj,m_ptrspace,sizeof(T) * m_arrylen);
        delete [] m_ptrspace;
        m_ptrspace = ptrNewObj;
    }

    m_ptrspace[m_arrylen++] = obj; //会调用[] 的重载函数
    //memcpy(&m_ptrspace[m_arrylen++],&obj,sizeof(T));
}
/**弹栈操作*/
template<typename T>
T modvector<T>::pop_back()
{
    if(0 == m_arrylen)
    {
        cout<<"vector is emtry"<<endl;
        return;
    }
   /* m_arrylen--;
    return m_ptrspace[m_arrylen];*/
    return m_ptrspace[--m_arrylen];
}
/**删除第index个元素*/
template<typename T>
 void modvector<T>::erase(int index)
 {
    if(index <0 || index >= m_arrylen)
    {
        cerr<<"访问越界"<<endl;
        return;
    }
    //m_arrylen--;
    for(int i = index; i < m_arrylen - 1;i++)
    {
        m_ptrspace[i] = m_ptrspace[i+1];
    }
    m_arrylen--;
 }
 /**清空容器*/
template<typename T>
void modvector<T>::clear()
{
    delete[] m_ptrspace;
    m_ptrspace = nullptr;
    m_arrylen = 0;
    m_capacity = DEF_CAP;

    m_ptrspace = new T[m_capacity];
}
#endif // MODVECTOR_H
