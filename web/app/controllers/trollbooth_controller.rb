class TrollboothController < ApplicationController
  ACK_TOPIC = 'trollbooth/acks'.freeze
  REQUEST_TOPIC = 'trollbooth/requests'.freeze

  def index; end

  def open
    head(open_gate ? :no_content : :service_unavailable)
  end

  def letmein
    @success = open_gate
  end

  private

  def open_gate
    message_id = SecureRandom.uuid
    message = nil
    mqtt do |client|
      client.subscribe ACK_TOPIC
      client.publish REQUEST_TOPIC, message_id
      wait_for_acknowledgement client
      _topic, message = client.get unless client.queue_empty?
    end

    message == message_id
  end

  def mqtt
    client = MQTT::Client.new
    client.host = Rails.application.config.mqtt_host
    client.client_id = 'trollbooth_server'
    client.connect
    yield client
    client.disconnect
  end

  def wait_for_acknowledgement(client)
    Timeout.timeout(5) { sleep 0.1 while client.queue_empty? }
  rescue Timeout::Error
    false
  end
end
