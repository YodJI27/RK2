## Рубежный контроль №2.


## Vector

  https://github.com/YodJI27/RK2 - ссылка на применяемый репозиторий.
  
  файл include/vector.hpp
  
  В строке 14 вызван конструктор, создающий вектор, типа T.
  
  ```ShellSession
  14   std::vector<T> Data;
  ```
  
  В строке 26 помещаем в созданный вектор 0.
  
  ```ShellSession
  26    Data.push_back(0);
  ```
  
  В строке 45 проводим действия с длиной вектора.
  
  ```ShellSession
  45    return Data.size() - 1;
  ```
  
  В строке 61, 70, 77 с помощью циклов обращемся к i-му элементу вектора.
  И проводим арифметическое действие ( += )
  
  ```ShellSession
  61    Data[i] += rhs.Data[i];
  
  77    Data[i + (m - rhs.m)] += rhs.Data[i];
  ```
   
  В строке 121, 130, 153 и т.д. происходит очистка вектора.
  
  ```ShellSession
  121    Data.clear();
  ```
  
  
  
  ## Queque
  
  файл include/tree.hpp
  
  В строке 207 cоздаем queque.
  
  ```ShellSession
  207    std::queue<typename BinarySearchTree<T>::Node*> queue;
  ```
   
  В строке 208 вставка элемента в очередь.
  
  ```ShellSession
  208    queue.push(tree.Root);
  ```
  
  В строке 212 получение элемента.
  
  ```ShellSession
  212    typename BinarySearchTree<T>::Node* temp = queue.front();
  ```
  
  В строке 213 удаление элемента из очереди.
  
  ```ShellSession
  213    queue.pop();
  ```
  
  
  ## Set
  
  файл include/Set.hpp
  
  В строке 82 создание set типа T.
  
  ```ShellSession
  82    std::set<T> output = a;
  ```
  
  В строке 83 добавление элемента в множество.
  
  ```ShellSession
  83    output.insert(b);
  ```
  
  Строка 99 возвращение итератора с помощью функции begin().
  
  ```ShellSession
  99    std::set_intersection(a.begin(), a.end(), 
                          b.begin(), b.end(),
                          std::inserter(output, output.begin()));
  ```       
  
  В строке 109 удаление элемента.
  
  ```ShellSession
  109    output.erase(b);
  ```
  
  
  ## Tree
  
  файл include/BinaryTree.hpp
  
  В строке 96 получение значения узла с перегруженного оператора*.
  
  ```ShellSession
  96    T operator*() const
  ```
  
  В строке 271 происходит обнуление дерева.
  
  ```ShellSession
  271    void clear()
     
  273    DeleteTree(root); 
  ```
  
  Получение крайнего правого и крайнего левого элемента происходит 
  с помощью функций TreeMin и TreeMax.
  
  ```ShellSession
  50    Node *TreeMin(Node *node)
  
  57    Node *TreeMax(Node *node)
  ```
  
  Получение ссылки на корень происходит с помощью поля root.
  
  ```ShellSession
  117    root;
  ```
