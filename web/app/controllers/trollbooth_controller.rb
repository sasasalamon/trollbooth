class TrollboothController < ApplicationController
  ACK_TOPIC = 'trollbooth/acks'.freeze
  REQUEST_TOPIC = 'trollbooth/requests'.freeze

  def index
  end

  def open
    open_gate do |success|
      head(success ? :no_content : :service_unavailable)
    end
  end

  def letmein
    open_gate do |success|
      @success = success
    end
  end

  private

  def open_gate
    mqtt do |client|
      message_id = SecureRandom.uuid
      client.subscribe ACK_TOPIC
      client.publish REQUEST_TOPIC, message_id
      wait_for_acknowledgement client
      _topic, message = client.get unless client.queue_empty?

      yield(message == message_id)
    end
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
