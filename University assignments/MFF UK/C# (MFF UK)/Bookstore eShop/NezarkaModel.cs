using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace NezarkaBookstore
{
	//
	// Model
	//
	class ModelStore
	{
		private List<Book> books = new List<Book>();
		private List<Customer> customers = new List<Customer>();

		public IList<Book> GetBooks()
		{
			return books;
		}

		public Book GetBook(int id)
		{
			return books.Find(b => b.Id == id);
		}

		public Customer GetCustomer(int id)
		{
			return customers.Find(c => c.Id == id);
		}

		public static ModelStore LoadFrom(TextReader reader)
		{
			var store = new ModelStore();

			try
			{
				if (reader.ReadLine() != "DATA-BEGIN")
				{
					return null;
				}
				while (true)
				{
					string line = reader.ReadLine();
					if (line == "")
					{
						return null;
					}
					else if (line == "DATA-END")
					{
						break;
					}
					string[] tokens = line.Split(';');
					switch (tokens[0])
					{
						case "BOOK":
							store.books.Add(new Book
							{
								Id = int.Parse(tokens[1]),
								Title = tokens[2],
								Author = tokens[3],
								Price = decimal.Parse(tokens[4])
							});
							break;
						case "CUSTOMER":
							store.customers.Add(new Customer
							{
								Id = int.Parse(tokens[1]),
								FirstName = tokens[2],
								LastName = tokens[3]
							});
							break;
						case "CART-ITEM":
							var customer = store.GetCustomer(int.Parse(tokens[1]));
							if (customer == null)
							{
								return null;
							}
							customer.ShoppingCart.Items.Add(new ShoppingCartItem
							{
								BookId = int.Parse(tokens[2]),
								Count = int.Parse(tokens[3])
							});
							break;
						default:
							return null;
					}
				}
			}
			catch (Exception ex)
			{
				if (ex is FormatException || ex is IndexOutOfRangeException)
				{
					return null;
				}
				throw;
			}
			return store;
		}
		public static void ExecuteCommands(string reader, ModelStore store, WebPage Web)
		{
			try
			{
				string line = reader;
				string[] input = line.Split(' ');
				if (input[0] != "GET")
				{
					Web.Error();
					return;
				}
				string[] command = input[2].Split('/');
				if (command[0] != "http:" || command[2] != "www.nezarka.net")
				{
					Web.Error();
					return;
				}
				var customer = store.GetCustomer(int.Parse(input[1]));
				if (customer == null)
				{
					Web.Error();
					return;
				}
				switch (command[3])
				{
					case "Books":
						if (command.Length == 4)
						{
							Web.Books(customer, store);
						}
						else if (command.Length == 6 && command[4] == "Detail")
						{
							Web.BookDetail(customer, store, store.GetBook(int.Parse(command[5])).Id);
						}
						else
						{
							Web.Error();
							return;
						}
						break;
					case "ShoppingCart":
						if (command.Length == 4)
						{
							if (customer.ShoppingCart.Items.Count > 0)
							{
								//neco ma
								Web.Casket(customer, store);
								return;
							}
							else
							{
								//prazdny
								Web.EmptyCasket(customer);
								return;
							}
						}
						else if (command[4] == "Add" && command.Length == 6)
						{
							if (store.GetBook(int.Parse(command[5])) == null)
							{
								Web.Error();
								return;
							}
							int length = customer.ShoppingCart.Items.Count;
							for (int i = 0; i < length; i++)
							{
								if (customer.ShoppingCart.Items[i].BookId == store.GetBook(int.Parse(command[5])).Id)
								{
									customer.ShoppingCart.Items[i].Count += 1;
									Web.Casket(customer, store);
									return;
								}
							}
							customer.ShoppingCart.Items.Add(new ShoppingCartItem
							{
								BookId = store.GetBook(int.Parse(command[5])).Id,
								Count = 1
							});
							Web.Casket(customer, store);
							return;
						}
						else if (command[4] == "Remove" && command.Length == 6)
						{
							int length = customer.ShoppingCart.Items.Count;
                            if (store.GetBook(int.Parse(command[5])) == null)
                            {
								Web.Error();
								return;
                            }
							for (int i = 0; i < length; i++)
							{
								if (customer.ShoppingCart.Items[i].BookId == store.GetBook(int.Parse(command[5])).Id)
								{
									customer.ShoppingCart.Items[i].Count -= 1;
									if (customer.ShoppingCart.Items[i].Count == 0)
									{
										customer.ShoppingCart.Items.RemoveAt(i);
									}
									Web.Casket(customer, store);
									return;
								}
							}
							Web.Error();
							return;
						}
						else
						{
							Web.Error();
							return;
						}

				}
			}
			catch (Exception e)
			{
				Web.Error();
			}
		}
	}

	class Book
	{
		public int Id { get; set; }
		public string Title { get; set; }
		public string Author { get; set; }
		public decimal Price { get; set; }
	}

	class Customer
	{
		private ShoppingCart shoppingCart;

		public int Id { get; set; }
		public string FirstName { get; set; }
		public string LastName { get; set; }

		public ShoppingCart ShoppingCart
		{
			get
			{
				if (shoppingCart == null)
				{
					shoppingCart = new ShoppingCart();
				}
				return shoppingCart;
			}
			set
			{
				shoppingCart = value;
			}
		}
	}
	class ShoppingCartItem
	{
		public int BookId { get; set; }
		public int Count { get; set; }
	}

	class ShoppingCart
	{
		public int CustomerId { get; set; }
		public List<ShoppingCartItem> Items = new List<ShoppingCartItem>();
	}
	class WebPage
	{
		List<StringBuilder> HTMLS = new List<StringBuilder> { };
		public void WriteOut()
		{
			foreach (var item in HTMLS)
			{
				Console.Write(item);
				Console.WriteLine("====");
			}
		}
		public void HeadPart(Customer customer, ref StringBuilder sb)
		{
			sb.Append("<!DOCTYPE html>\n");
			sb.Append("<html lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\">\n");
			sb.Append("<head>\n");
			sb.Append("	<meta charset=\"utf-8\" />\n");
			sb.Append("	<title>Nezarka.net: Online Shopping for Books</title>\n");
			sb.Append("</head>\n");
			sb.Append("<body>\n");
			sb.Append("	<style type=\"text/css\">\n");
			sb.Append("		table, th, td {\n");
			sb.Append("			border: 1px solid black;\n");
			sb.Append("			border-collapse: collapse;\n");
			sb.Append("		}\n");
			sb.Append("		table {\n");
			sb.Append("			margin-bottom: 10px;\n");
			sb.Append("		}\n");
			sb.Append("		pre {\n");
			sb.Append("			line-height: 70%;\n");
			sb.Append("		}\n");
			sb.Append("	</style>\n");
			sb.Append("	<h1><pre>  v,<br />Nezarka.NET: Online Shopping for Books</pre></h1>\n");
			sb.Append("	" + customer.FirstName + ", here is your menu:\n");
			sb.Append("	<table>\n");
			sb.Append("		<tr>\n");
			sb.Append("			<td><a href=\"/Books\">Books</a></td>\n");
			sb.Append("			<td><a href=\"/ShoppingCart\">Cart (" + customer.ShoppingCart.Items.Count + ")</a></td>\n");
			sb.Append("		</tr>\n");
			sb.Append("	</table>\n");
		}
		public void EmptyCasket(Customer customer)
		{
			StringBuilder sb = new StringBuilder();
			HeadPart(customer, ref sb);
			sb.Append("\tYour shopping cart is EMPTY.\r\n");
			sb.Append("</body>\r\n");
			sb.Append("</html>\r\n");
			HTMLS.Add(sb);
		}
		public void Casket(Customer customer, ModelStore store)
		{
			StringBuilder sb = new StringBuilder();
			HeadPart(customer, ref sb);
			sb.Append("\tYour shopping cart:\r\n");
			sb.Append("\t<table>\r\n");
			sb.Append("\t\t<tr>\r\n");
			sb.Append("\t\t\t<th>Title</th>\r\n");
			sb.Append("\t\t\t<th>Count</th>\r\n");
			sb.Append("\t\t\t<th>Price</th>\r\n");
			sb.Append("\t\t\t<th>Actions</th>\r\n");
			sb.Append("\t\t</tr>\r\n");
			decimal sum = 0;
			foreach (var item in customer.ShoppingCart.Items)
			{
				sum += store.GetBook(item.BookId).Price * item.Count;
				sb.Append("\t\t<tr>\r\n");
				sb.Append("\t\t\t<td><a href=\"/Books/Detail/" + item.BookId + "\">" + store.GetBook(item.BookId).Title + "</a></td>\r\n"); ;
				sb.Append("\t\t\t<td>" + item.Count + "</td>\r\n");
				if (item.Count > 1)
				{
					sb.Append("\t\t\t<td>" + item.Count + " * " + store.GetBook(item.BookId).Price + " = " + item.Count * store.GetBook(item.BookId).Price + " EUR" + "</td>\r\n");
				}
				else
				{
					sb.Append("\t\t\t<td>" + store.GetBook(item.BookId).Price + " EUR" + "</td>\r\n");
				}
				sb.Append("\t\t\t<td>&lt;<a href=\"/ShoppingCart/Remove/" + store.GetBook(item.BookId).Id + "\">Remove</a>&gt;</td>\n");
				sb.Append("\t\t</tr>\r\n");
			}
			sb.Append("\t</table>\r\n");
			sb.Append("\tTotal price of all items: " + sum + " EUR\r\n");
			sb.Append("</body>\r\n");
			sb.Append("</html>\r\n");
			HTMLS.Add(sb);
		}
		public void BookDetail(Customer customer, ModelStore store, int ID)
		{
			StringBuilder sb = new StringBuilder();
			HeadPart(customer, ref sb);
			sb.Append("\tBook details:\r\n");
			sb.Append("\t<h2>" + store.GetBook(ID).Title + "</h2>\r\n");
			sb.Append("\t<p style=\"margin-left: 20px\">\r\n");
			sb.Append("\tAuthor: " + store.GetBook(ID).Author + "<br />\r\n");
			sb.Append("\tPrice: " + store.GetBook(ID).Price + " EUR<br />\r\n");
			sb.Append("\t</p>\r\n");
			sb.Append("\t<h3>&lt;<a href=\"/ShoppingCart/Add/" + store.GetBook(ID).Id + "\">Buy this book</a>&gt;</h3>\n");
			sb.Append("</body>\r\n");
			sb.Append("</html>\r\n");
			HTMLS.Add(sb);
		}
		public void Error()
		{
			StringBuilder sb = new StringBuilder();
			sb.Append("<!DOCTYPE html>\r\n");
			sb.Append("<html lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\">\r\n");
			sb.Append("<head>\r\n");
			sb.Append("\t<meta charset=\"utf-8\" />\r\n");
			sb.Append("\t<title>Nezarka.net: Online Shopping for Books</title>\r\n");
			sb.Append("</head>\r\n");
			sb.Append("<body>\r\n");
			sb.Append("<p>Invalid request.</p>\r\n");
			sb.Append("</body>\r\n");
			sb.Append("</html>\r\n");
			HTMLS.Add(sb);
		}
		public void Books(Customer customer, ModelStore store)
		{
			StringBuilder sb = new StringBuilder();
			int knizka = 0;
			HeadPart(customer, ref sb);
			sb.Append("	Our books for you:\n");
			sb.Append("	<table>\n");
			sb.Append("		<tr>\n");
			foreach (var item in store.GetBooks())
			{
				if (knizka == 3)
				{
					sb.Append("\t\t</tr>\r\n");
					sb.Append("\t\t<tr>\r\n");
					knizka = 0;
				}
				sb.Append("\t\t\t<td style=\"padding: 10px;\">\r\n");
				sb.Append("\t\t\t\t<a href=\"/Books/Detail/" + item.Id + "\">" + item.Title + "</a><br />\r\n");
				sb.Append("\t\t\t\tAuthor: " + item.Author + "<br />\r\n");
				sb.Append("\t\t\t\tPrice: " + item.Price + " EUR &lt;<a href=\"/ShoppingCart/Add/" + item.Id + "\">Buy</a>&gt;\r\n");
				sb.Append("\t\t\t</td>\r\n");
				knizka++;
			}
			sb.Append("\t\t</tr>\r\n");
			sb.Append("\t</table>\r\n");
			sb.Append("</body>\r\n");
			sb.Append("</html>\r\n");
			HTMLS.Add(sb);
		}
	}
}