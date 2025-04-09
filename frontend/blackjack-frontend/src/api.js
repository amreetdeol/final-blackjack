const BASE_URL = "http://localhost:8080";

export async function post(path, body, token = "") {
  try {
    const res = await fetch(`${BASE_URL}${path}`, {
      method: "POST",
      headers: {
        "Content-Type": "text/plain",
        "Authorization": token,
      },
      body,
    });
    return await res.text();
  } catch (err) {
    console.error(err);
    return "❌ Failed to reach server";
  }
}