class List {
	item: String;
	next: List;

	init(i: String, n: List): List {
		{
			item <- i;
			next <- n;
			self;
		}
	};

	flatten(): String {
		if (isvoid(next)) then
			item
		else
			item.concat(flatten(next))
		fi
	};
};

class Main inherits IO {

	main() :Object {
		let hello: String <- "hello",
			world: String <- "world !.",
			newline: String <- "\n",
			nil: List,
			list: List <- 
				(new List).init(hello,
					(new List).init(world,
				 	    (new List).init(newline, nil)))
		in 
			out_string(hello.concat(world.concat(newline)))
			out_string(list.flatten())
	};
};