const BASE_URL = "https://blackjack-backend-310062056748.us-central1.run.app";

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