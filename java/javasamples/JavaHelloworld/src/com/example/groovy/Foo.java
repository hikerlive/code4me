package com.example.groovy;

/**
 * Created with IntelliJ IDEA.
 * User: Administrator
 * Date: 12-12-24
 * Time: 下午3:04
 * To change this template use File | Settings | File Templates.
 */
public class Foo {
    doubleSomething() {
        data = ["name": "James", "location": "London"]
        for (e in data) {
            println("entry ${e.key} is ${e.value}")
        }
    }

    closureExample(collection) {
        collection.each { println("value ${it}") }
    }

    static void main (args) {
        values = [1, 2, 3, "abc"]
        foo = new Foo()
        foo.closureExample(values)
        foo.doSomething()
    }
}
