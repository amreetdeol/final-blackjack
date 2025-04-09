import React, { useState } from "react";
import { post } from "../api";

export default function LoginForm({ onLogin }) {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");

  const handleLogin = async () => {
    await post("/register", `${username}:${password}`); // ignore failure if already registered
    const session = await post("/login", `${username}:${password}`);
    if (session) {
      onLogin(username, session);
      setUsername("");
      setPassword("");
    } else {
      alert("Login failed");
    }
  };

  return (
    <div style={{ marginBottom: "1rem" }}>
      <input value={username} onChange={(e) => setUsername(e.target.value)} placeholder="Username" />
      <input value={password} onChange={(e) => setPassword(e.target.value)} placeholder="Password" type="password" />
      <button onClick={handleLogin}>Enter Game</button>
    </div>
  );
}