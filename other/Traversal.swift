class List<Element> {
	class Node {
		var value: Element
		var next : Node? = nil

		init(value: Element) {
			self.value = value
		}
	}

	var head: Node? = nil
	var tail: Node? = nil

	func empty () -> Bool {
		if head != nil {
			return false
		}
		return true
	}

	func pushFront (value: Element) {
		let newNode = Node (value: value)

		if head != nil {
			newNode.next = head
			head         = newNode
		} else {
			head = newNode
			tail = newNode
		}
	}

	func pushBack (value: Element) {
		let newNode = Node (value: value)

		if tail != nil {
			tail!.next = newNode
			tail       = newNode
		} else {
			head = newNode
			tail = newNode
		}
	}

	func popFront () -> Element? {
		if head != nil {
			let value = head!.value
			head      = head!.next

			if head == nil {
				tail = nil
			}

			return value
		} else {
			return nil
		}
	}
}


protocol Container {
	associatedtype Element

	func push (value: Element)
	func pop  () -> Element?

	func empty () -> Bool
}

class Queue<Element>: Container {
	var list = List<Element>()

	func push(value: Element) {
		list.pushBack(value: value)
	}

	func pop() -> Element? {
		return list.popFront()
	}

	func empty () -> Bool {
		return list.empty()
	}
}

class Stack<Element>: Container {
	var list = List<Element>()

	func push(value: Element) {
		list.pushFront(value: value)
	}

	func pop() -> Element? {
		return list.popFront()
	}

	func empty () -> Bool {
		return list.empty()
	}
}


class Graph<ElementType: Hashable> {
	class Node: Hashable {
		var value: ElementType
		var neighbors = [Node] ()

		init (value: ElementType) {
			self.value = value
		}

		func link (with newNeighbors: [Node]) {
			for node in newNeighbors {
				if node != self  &&
				isLinked (with: node) == false {
					self.neighbors.append(node)
					node.neighbors.append(self)
				}
			}
		}

		func isLinked (with node: Node) -> Bool {
			if neighbors.first(where: { $0 == node }) != nil {
    			return true
			}
			return false
		}

		static func == (lhs: Node, rhs: Node) -> Bool {
			if  lhs.value     == rhs.value &&
				lhs.neighbors == rhs.neighbors {
				return true
			}

			return false
		}

		var hashValue: Int {
			return value.hashValue
		}
	}

	var nodes = [Node]()

	func makeNode (value: ElementType) -> Node {
		let newNode = Node(value: value)

		nodes.append(newNode)

		return newNode
	}

	enum TraversingMethod {
		case DFS
		case BFS
	}

	private func search<CT: Container>(value: ElementType, with container: CT, from root: Node) -> [ElementType]
	where CT.Element == Node {
		var parentMap    = [Node: Node?]()
		var passedNodes  = Set<Node>()
		var found: Node? = nil

		parentMap[root] = nil
		container.push(value: root)

		while container.empty() == false {
			let currNode = container.pop()

			if currNode!.value == value {
				found = currNode
				break
			}

			passedNodes.insert(currNode!)

			for neighbor in currNode!.neighbors {
				if passedNodes.contains(neighbor) == false {
					if parentMap[neighbor] == nil {
						parentMap[neighbor] = currNode
					}

					container.push(value: neighbor)
				}
			}
		}

		if found == nil {
			return []
		}

		var pathway = [ElementType]()
		let reversedPathway = Stack<Node>()

		while parentMap[found!] != nil {
			reversedPathway.push(value: found!)
			found = parentMap[found!]!
		}
		reversedPathway.push(value: found!)

		while reversedPathway.empty() == false {
			pathway.append(reversedPathway.pop()!.value)
		}

		return pathway
	}


	func search(value: ElementType, by method: TraversingMethod, from root: Node) -> [ElementType] {
		switch method {
		case .DFS:
			let container = Stack<Node>()
			return search(value: value, with: container, from: root)
		case .BFS:
			let container = Queue<Node>()
			return search(value: value, with: container, from: root)
		}
	}
}

var graph = Graph<String>()

var A = graph.makeNode(value: "A")
var B = graph.makeNode(value: "B")
var C = graph.makeNode(value: "C")
var D = graph.makeNode(value: "D")
var E = graph.makeNode(value: "E")
var F = graph.makeNode(value: "F")
var G = graph.makeNode(value: "G")

A.link(with: [B, F])
B.link(with: [C])
C.link(with: [D, E])
E.link(with: [G, F])

print(graph.search(value: "G", by: .BFS, from: A))
