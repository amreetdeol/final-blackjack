import { useState } from "react";
import LoginForm from "./components/LoginForm";
import GameControls from "./components/GameControls";

export default function App() {
  const [players, setPlayers] = useState([]);

  const handleLogin = (username, session) => {
    setPlayers((prev) => [...prev, { username, session }]);
  };

  return (
    <div style={{ padding: "2rem", fontFamily: "sans-serif" }}>
      <h1>🃏 Multiplayer Blackjack</h1>
      <LoginForm onLogin={handleLogin} />

      <div style={{ display: "flex", flexWrap: "wrap", gap: "1rem", marginTop: "2rem" }}>
        {players.map((player, idx) => (
          <GameControls key={idx} username={player.username} session={player.session} />
        ))}
      </div>
    </div>
  );
}