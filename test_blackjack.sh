#!/usr/bin/env bash

BASE_URL="http://localhost:8080"
MAX_PLAYERS=4

USERNAMES=()
SESSIONS=()

echo "🟢 Registering and logging in players..."

for ((i = 1; i <= MAX_PLAYERS; i++)); do
  username="player$i"
  password="pass$i"
  USERNAMES[$i]=$username

  echo "🔹 Player $i ($username):"

  # Try register
  register_response=$(curl -s -X POST -H "Content-Type: text/plain" --data "$username:$password" "$BASE_URL/register")
  echo "   📝 Register: $register_response"

  # Try login
  login_response=$(curl -s -X POST -H "Content-Type: text/plain" --data "$username:$password" "$BASE_URL/login")
  token=$(echo "$login_response" | tr -d '\r\n')

  if [[ "$token" == session-* ]]; then
    SESSIONS[$i]=$token
    echo "   ✅ Login success: $token"
  else
    echo "   ❌ Login failed: $login_response"
  fi
done

echo ""

# Game interaction loop
while true; do
  echo "🎮 Choose player number (1-$MAX_PLAYERS), or 'q' to quit:"
  read -r player
  [[ "$player" == "q" ]] && break

  if ! [[ "$player" =~ ^[1-9][0-9]*$ ]] || (( player < 1 || player > MAX_PLAYERS )); then
    echo "❌ Invalid player number."
    continue
  fi

  token="${SESSIONS[$player]}"
  if [[ -z "$token" ]]; then
    echo "❌ No session found for player $player (${USERNAMES[$player]})."
    continue
  fi

  echo "🔧 Enter action: deal, hit, stand, status"
  read -r action

  case "$action" in
    deal|hit|stand|status)
      echo ">>> Player $player (${USERNAMES[$player]}): $action"
      response=$(curl -s -X POST \
        -H "Content-Type: text/plain" \
        -H "Authorization: $token" \
        "$BASE_URL/$action")
      echo "$response"
      ;;
    *)
      echo "❌ Invalid action."
      ;;
  esac
done