int sha1() {
  filtered_val = analogRead(IN5);
  V = filtered_val * 0.0048828125;
  distance = 16 * pow(V, -1.10);
  distance = constrain(distance, 3, 150);
  return distance;
}

int sha2() {
  filtered_val = analogRead(IN6);
  V = filtered_val * 0.0048828125;
  distance = 16 * pow(V, -1.10);
  distance = constrain(distance, 3, 150);
  return distance;
}