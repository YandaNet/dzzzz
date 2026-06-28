using System;
using System.Net.Http;
using System.Text.Json;
using System.Threading.Tasks;

namespace DuckDuckGoSearch
{
    class Program
    {
        static async Task Main(string[] args)
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;

            Console.Write("Введіть пошуковий запит: ");
            string query = Console.ReadLine();

            string url =
                $"https://api.duckduckgo.com/?q={query}&format=json";

            HttpClient client = new HttpClient();

            try
            {
                string json = await client.GetStringAsync(url);

                JsonDocument doc = JsonDocument.Parse(json);

                JsonElement relatedTopics =
                    doc.RootElement.GetProperty("RelatedTopics");

                Console.WriteLine("\nРезультати:\n");

                foreach(JsonElement item in relatedTopics.EnumerateArray())
                {
                    // Іноді є вкладені Topics
                    if (item.TryGetProperty("Topics", out JsonElement topics))
                    {
                        foreach(JsonElement subItem in topics.EnumerateArray())
                        {
                            ShowResult(subItem);
                        }
                    }
                    else
                    {
                        ShowResult(item);
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Помилка: " + ex.Message);
            }

            Console.ReadKey();
        }

        static void ShowResult(JsonElement item)
        {
            if (item.TryGetProperty("Text", out JsonElement text) &&
                item.TryGetProperty("FirstURL", out JsonElement url))
            {
                Console.WriteLine("Текст:");
                Console.WriteLine(text.GetString());

                Console.WriteLine("Посилання:");
                Console.WriteLine(url.GetString());

                Console.WriteLine(new string('-', 50));
            }
        }
    }
}