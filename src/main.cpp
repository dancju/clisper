#include<algorithm>
#include<cassert>
#include<iostream>
#include<list>
#include<map>
#include<vector>

#include"tokenizer.cpp"
#include"parser.cpp"
#include"evaluater.cpp"

int main() {
  tokenize(std::cin);
  // for_each(tokens.begin(), tokens.end(), [](std::string &buff) { std::cout << buff << std::endl; });
  add_symbol("t");
  add_symbol("nil");
  add_symbol("'");
  add_symbol("quote");
  add_symbol("atom");
  add_symbol("eq");
  add_symbol("car");
  add_symbol("cdr");
  add_symbol("cons");
  add_symbol("cond");
  add_symbol("lambda");
  add_symbol("label");
  for_each(tokens.begin(), tokens.end(), [&](const std::string& s) {
    nodes.push_back(node(s));
  });
  for(auto i = nodes.begin(); i!=nodes.end(); i++) {
    parse(i);
  }
  for_each(nodes.begin(), nodes.end(), [&](node &n) {
    if(eval(n)) {
      print(n);
      putchar('\n');
    }
  });
  return 0;
}
